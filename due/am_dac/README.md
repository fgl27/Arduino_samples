# Transmissor AM usando Arduíno projeto em andamento da faculdade

Neste usamos um Arduíno DUE

![CI](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci.png)

Este transmissor AM digital funciona como qualquer outro transmissor AM comercial digital

Em uma modulação AM temos a imagem abaixo, o sinal modulado varia sua amplitude em relação ao sinal modulador ou sinal de áudio a modular.

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/am_mod.png)

Neste projeto para modular o sinal foi usado esta mesma técnica mas a portadora é uma onda quadrada, assim o resultado do sinal modulado é o visto na imagem abaixo, sendo o sinal de mensagem uma onda senoidal que oscila entre 0 e um valor positivo de tensão

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/modulado.png)

# Explicação detalhada funcionamento do processo lógico:

Assim para gerar esta modulação no microcontrolador temos os seguintes passos [Configuração do Arduíno esta no arquivo am_dac.ino](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/am_dac.ino)

* A função **setup()** inicializa todas funções;
* A função **adc_setup()** Configura o ADC, habilita o ADC A0;
* A função **dac_setup()** Configura o DAC, habilita o DAC1;
* A função **tc_setup()** Configura o timer e os contadores, habilita o TC2 para ser usado como interrupção de leitura do ADC;
* A função **ADC_Handler()** é a interrupção do ADC, nesta lê o ADC a cada pulso par to timer e seta o valor do ADC no DAC, em pulsos impares zera o valor do DAC sem ler o ADC, segundo Datasheet não é recomendado fazer leituras do ADC com frequências superiores a 1 MHz, pois o resultado é incerto da leitura;
* Assim obtendo a modulação AM, ou seja simula o processo de multiplicar uma portadora de onda quadrada pela mensagem usando a logica do Arduíno Due;
* A frequências suportadas de 100Hz ate 807.44kHz, devido tanto o ADC quando o DAC terem limites da frequência de leitura e escrita, esta foi a máxima frequência possível de obter resultado.
* Referencia da configuração usada consulte o [Datasheet SAM3X / SAM3A Series](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf), neste tem a descrição do que é cada registro, função e parâmetro usado.

O resultado deste processo pode ser visto na pratica na imagem abaixo:

Temos [um sinal de áudio de um tom senoidal 1kHz](https://www.youtube.com/watch?v=3FBijeNg_Gs) na entrada do circuito.

O canal 0 do osciloscópio (sinal amarelo) esta conectado na porta do DAC, oscilando a uma frequência de 700kHz.<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio original com um offset visto pela porta do ADC.<br>

Como é possível ver o sinal modulado acompanha perfeitamente o sinal da mensagem.

![1k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/tone_1_kHz.png)

# Explicação detalhada funcionamento do circuito:

![CI](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci.png)

O funcionamento do circuito ao redor do Arduíno é muito simples.

* Na entrada de áudio foi conectada a saída de áudio de um notebook, este sinal segue ate o capacitor
* O capacitor de 10uF funciona apenas como um filtro bloqueando sinal CC deixando passar apenas o sinal de áudio analógico vindo do entrada de áudio.
* Os dois resistores de 10kΩ formão um divisor de tensão, divide a tensão referencia do ADC pela metade
* O sinal de áudio esta oscilando sobre 0V, ou seja, para mais e para menos de 0V com um pico de amplitude de 0,5V (Ajustado através do volume da fonte de audio), quando este é conectado ao capacitor duas coisas ocorrem, a parte CC do sinal é filtrada e o sinal é elevado em tensão (um offset para mais) para agora oscilar abaixo e acima de **VREF 3.3V/2 = 1,65V**, assim temos o resultado do sinal visto na imagem do osciloscópio
* Assim este sinal filtrado e elevado é conectado na porta **A0** dos ADC
* Temos um timer configurado a 1.4 MHz
* O Arduíno então a cada pulso impar do timer zera o DAC, e pulsos pares lê o valor do ADC e seta este valor no DAC
* E na porta **DAC1** temos a saída o sinal modulado AM porem ainda não filtrado
* Assim temos um filtro passa banda conectado no DAC, formada por um filtro passa alta com frequência de corte 692kHz e um passa baixa frequência de corte 708kHz
* E na saída do filtro conectado a um amplificador classe A, amplifica a potencia do sinal o suficiente para termos um sinal claro no radio, pois ao passar pelo filtro perdemos um pouco de potencia no sinal
* A saída do amplificador conectado a antena

# Filtragem:

Para obter um sinal o mais próximo de um sinal AM comercial, é necessário aplicar um filtro na saída do DAC, o filtro tem algumas funções:

* Eliminar a componente CC do sinal
* Eliminar ou diminuir a contribuição de frequências além da fundamental (700 kHz) para o sinal
* Limitar a largura de banda do sinal
* Obter um sinal o mais próximo de uma AM comercial

Como o objetivo é obter um sinal modulado em amplitude muito próximo do que é mostrado na imagem abaixo temos de fazer a filtragem.

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/am_mod.png)

Assim podemos ver o efeito que o filtro causa no sinal nas imagens abaixo, sem filtragem o sinal não é um AM puro, apos o filtro o sinal esta bem próximo de um AM modulado como o da imagem em exemplo acima.

Sem filtragem:

O canal 0 do osciloscópio (sinal amarelo) esta conectado na saída do DAC, oscilando a uma frequência de 700kHz.<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio (um tom senoidal 1kHz) com um offset, sinal visto pela porta do ADC.<br>
FFT(0) do osciloscópio (expectro amarelo) é o espectro FFT do sinal do DAC.<br>

![out_antes_fitro](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/fft_1kz_dac.png)

Com filtragem:

O canal 0 do osciloscópio (sinal amarelo) esta conectado na saída do filtro, oscilando a uma frequência de 700kHz.<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio (um tom senoidal 1kHz) com um offset, sinal visto pela porta do ADC.<br>
FFT(0) do osciloscópio (expectro amarelo) é o espectro FFT do sinal do filtro RLC.<br>

![out_depois_filtro](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/fft_1kz_filtro.png)

Como é possível ver os picos no espectro de Fourier a frequancia 0, 200, 500 e 600 kHz são eliminados, o pico em 100 kHz diminui bastante, e a energia do sinal se concentra na frequência de operação 700 kHz.

# Escolha do filtro:

Neste foi proposto e testado dois filtros passa banda, um RC e outro RLC, ambos funcionam porem o que demostra melhor resultado é o filtro RLC, pois este consegue entregar um sinal mais próximo de uma senoide quando comparado ao sinal do filtro RC, um sinal senoidal é o mais ideal, pois a potencia do sinal fica concentrada muito mais na frequência principal, assim evitando sinal fora da banda de operação e caso o sinal fosse amplificado evita amplificar sinais indesejados.<br>

Nas imagens abaixo temos uma visualização dos resultado de cada filtro

Zoom do sinal do DAC

![zoom_dac](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/zoom_dac.png) 

Zoom do DAC apos o Filtro RC

![zoom_filtro_rc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/zoom_filtro_rc.png) 

Zoom do DAC apos o Filtro RLC

![zoom_filtro_rlc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/zoom_filtro_rlc.png) 

FFT Filtro RC

![FFT_filtro_rc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/FFT_filtro_rc.png) 

FFT Filtro RLC

![FFT_filtro_rlc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/FFT_filtro_rlc.png)

Como é possível ver o sinal do filtro RC não é uma senoide, mas o sinal do RLC é muito próximo de uma senoide. Pelo espectro da FFT vemos que no filtro RLC a energia se concentra próxima a frequência de operação 700 kHz, já no filtro RC temos um pico em 600 kHz e o pico em 100 kHz é bem maior

# Amplificador:

Sem amplificador temos um leve ruido ao escutar o áudio em um radio (confira video demostrativo no final deste), assim foi adicionado este amplificador classe A, o mais simples amplificador possível, alimentado pelo próprio Arduíno e foi obtido sucesso em remover o ruido sem adicionar problemas, como outros ruídos e amplificar frequências indesejadas, o ganho de tensão ficou em 2,35 vezes.

Abaixo a comparação dos sinais com e sem amplificação, no canal 0 em amarelo o sinal amplificado, no canal 1 em vermelho sem amplificação.

Visão geral:

![amp_geral](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/amp_geral.png)

Zoom:

![amp_zoom](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/amp_zoom.png)

# Imagens 3d circuito e PCB Arduíno:

[PCB veja este pdf](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/pcb.pdf)

![ci_3d](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci_3d.png)

![ci_3d_2](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci_3d_3.png)

![ci_3d_2](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci_3d_2.png)

# Imagens do circuito montado para test no Arduíno:

Com filtro RLC e amplificador

![CI_DONE1](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_4.jpg?raw=true)

![CI_DONE2](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_5.jpg?raw=true)


Com filtro RLC sem amplificador

![CI_DONE1](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_2.jpg?raw=true)

![CI_DONE2](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_3.jpg?raw=true)

Com filtro RC sem amplificador

![CI_DONE3](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_0.jpg?raw=true)

![CI_DONE4](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_1.jpg?raw=true)

# Resultados de alguns sinais de áudio:

O canal 0 do osciloscópio (sinal amarelo) esta conectado na saída do filtro RLC, oscilando a uma frequência de 700kHz (frequência de operação pode variar um pouco nas imagens a seguir, confira as imagens de zoom anterior para ver que esta oscilando a 700 kHz fixo).<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio com um offset visto pela porta do ADC.<br>

1kHz https://www.youtube.com/watch?v=3FBijeNg_Gs

![1k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/tone_1_kHz.png)

3kHz https://www.youtube.com/watch?v=CAwpIiDQv5w

![3k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/tone_3_kHz.png)

7kHz https://www.youtube.com/watch?v=3_ciJKxe9VU

![7k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/tone_7_kHz.png)

1kHz + 3kHz + 7kHz ... todos habilitados ao mesmo tempo no notebook

![soma](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/soma_tom_1_3_7_kHz.png)

# Video demostrativos do projeto funcionando num rádio

No video abaixo uma breve explicação do funcionamento e como é possível ver a musica fica com uma qualidade de áudio muito boa para AM.

[Video demostrativos filtro RLC com amplificação](https://drive.google.com/file/d/190R10johRux9wrGtr1tya3P-hCyCnH-G/view?usp=sharing)

[Video demostrativos filtro RLC sem amplificação](https://drive.google.com/file/d/1aLIixW98E_zOYSKfy_qHgwoAFog0OWmn/view?usp=sharing)

[Video demostrativos filtro RC sem amplificação](https://drive.google.com/file/d/1iUHsBG3t269ETMHgweM_uz2gOnA2Elhb/view?usp=sharing)

