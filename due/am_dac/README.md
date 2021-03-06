**Transmissor AM usando Arduíno projeto em andamento da faculdade**

- [Introdução:](#introdução)
- [Desenvolvimento:](#desenvolvimento)
  - [Explicação detalhada funcionamento do circuito:](#explicação-detalhada-funcionamento-do-circuito)
  - [Explicação detalhada funcionamento do processo lógico:](#explicação-detalhada-funcionamento-do-processo-lógico)
  - [Filtragem:](#filtragem)
    - [Sinal sem filtragem:](#sinal-sem-filtragem)
    - [Sinal com filtragem:](#sinal-com-filtragem)
  - [Escolha do filtro:](#escolha-do-filtro)
    - [Zoom do sinal do DAC](#zoom-do-sinal-do-dac)
    - [Zoom do DAC apos o Filtro RC](#zoom-do-dac-apos-o-filtro-rc)
    - [Zoom do DAC apos o Filtro RLC](#zoom-do-dac-apos-o-filtro-rlc)
    - [FFT Filtro RC](#fft-filtro-rc)
    - [FFT Filtro RLC](#fft-filtro-rlc)
  - [Amplificador:](#amplificador)
    - [Sinal visão geral:](#sinal-visão-geral)
    - [Sinal com Zoom:](#sinal-com-zoom)
    - [FFT saída do filtro:](#fft-saída-do-filtro)
  - [Imagens 3d circuito e PCB Arduíno:](#imagens-3d-circuito-e-pcb-arduíno)
  - [Imagens do circuito montado para test no Arduíno:](#imagens-do-circuito-montado-para-test-no-arduíno)
- [Apresentação](#apresentação)
  - [Resultados de alguns sinais de áudio:](#resultados-de-alguns-sinais-de-áudio)
  - [Video demostrativos do projeto funcionando num rádio](#video-demostrativos-do-projeto-funcionando-num-rádio)
- [Conclusão](#conclusão)

## Introdução:

Para desenvolver este projeto escolhemos usar um processo de modulação digital chaveada usando Arduíno. Este processo é exemplificado no diagrama abaixo.

![mod_chave](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/mod_chave.png)

Neste usamos um Arduíno DUE que tem alumas qualidades de um FPGA mas é considerado apenas um microcontrolador.

![arduinodue](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduinodue.jpg?raw=true)

Este transmissor AM digital funciona como qualquer outro transmissor AM comercial digital

Em uma modulação AM temos a imagem abaixo, o sinal modulado varia sua amplitude em relação ao sinal modulador ou sinal de áudio a modular.

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/am_mod.png)

Neste projeto para modular o sinal foi usado esta mesma técnica mas a portadora é uma onda quadrada, assim o resultado do sinal modulado é o visto na imagem abaixo, sendo o sinal de mensagem uma onda senoidal que oscila entre 0 e um valor positivo de tensão

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/modulado.png)

Como o sinal gerado na saída do Arduíno não é um sinal senoide, ou seja, não é um sinal AM comercial, é necessário adicionar um filtro e no nosso caso um amplificador, pois o sinal apos o filtragem ficou com baixa potência, para que tenhamos um sinal adequado para transmissão.

Assim no desenvolvimento vamos demostra como usamos a técnica de modulação aprendidas neste semestre para fazer este transmissor funcionar de forma adequada como um transmissor AM comercial.

## Desenvolvimento:

O desenvolvimento foi feito em etapas:

* Primeiro desenvolvemos um circuito com base no exemplo do modulador digital;
* Apos desenvolvemos uma logica capaz de produzir a modulação;
* Apos conseguir fazer funcionar nem tudo ficou perfeito, assim, aplicamos os conhecimentos adquiridos na cadeira
* Com o auxilio do professor através dos fóruns durante as aulas foi tirada as duvidas em relação a filtragem do sinal e assim foi desenvolvido um filtro adequado para o projeto;
* Apos introduzir o filtro foi percebido que a potencia do sinal não estava adequada, o que gerava ruídos sonoro notável no radio, assim foi adicionado um simples amplificador;
* Assim com tudo funcionando foi desenvolvida um PCB final com todos componentes necessário para funcionamento deste.

Abaixo temos cada um destes passos em detalhes.

### Explicação detalhada funcionamento do circuito:

![CI](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci.png)

O funcionamento do circuito ao redor do Arduíno é muito simples.

* Na entrada de áudio foi conectada a saída de áudio de um notebook, este sinal segue ate o capacitor
* O capacitor de 10uF funciona apenas como um filtro bloqueando a corrente CC do sinal deixando passar apenas o sinal de áudio analógico vindo da fonte de áudio.
* Os dois resistores de 10kΩ formão um divisor de tensão, divide a tensão referencia do ADC 3,3V pela metade
* O sinal de áudio esta oscilando sobre 0V, ou seja, para mais e para menos de 0V com um pico de amplitude de 0,5V (Ajustado através do volume da fonte de audio), quando este é conectado ao capacitor duas coisas ocorrem, a parte CC do sinal é filtrada e o sinal é elevado em tensão (um offset para mais) para agora oscilar abaixo e acima de **VREF 3.3V/2 = 1,65V**, assim temos o resultado do sinal visto na imagem do osciloscópio
* Assim este sinal filtrado e com um offset é conectado na porta **A0** dos ADC
* Temos um timer configurado a 1.4 MHz
* O Arduíno então a cada pulso impar do timer zera a saída do DAC, e pulsos pares lê o valor do ADC e seta este valor na saída do DAC
* E na porta **DAC1** temos a saída o sinal modulado AM porem ainda não filtrado
* Assim temos um filtro passa banda conectado no DAC, formada por um filtro passa alta com frequência de corte 692kHz e um passa baixa frequência de corte 708kHz, ou seja, com uma banda de 16kHz, o ajuste da banda é feito apenas mudando o valor da resistência do trimpot utilizado, neste caso foi usada um trimpot de ate 100 Ω ajustado para 9 Ω
* O valor nominal e medido do indutor foi de 90µH, o valor nominal do capacitor é 561 pF e medido 572 pF, o que na prática desloca o filtro 1.5kHz para cima, ou seja, as frequências de corte ficam 693.5 a 709.5 kHz usando um resistor de 9 Ω, o que não afeta o funcionamento prático
* E na saída do filtro conectado a um amplificador classe A, simples apenas com transistor e resistores, amplifica a potencia do sinal o suficiente para termos um sinal de áudio claro sem ruídos audíveis no radio.
* A saída do amplificador conectado a antena, que é um simples fio de alguns metros

![calc_filtro_rlc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/calc_filtro_rlc.png)


### Explicação detalhada funcionamento do processo lógico:

Assim para gerar esta modulação no microcontrolador temos os seguintes passos [Configuração do Arduíno esta no arquivo am_dac.ino](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/am_dac.ino)

* A função **setup()** inicializa todas funções;
* A função **adc_setup()** Configura o ADC, habilita o ADC **A0**;
* A função **dac_setup()** Configura o DAC, habilita o **DAC1**;
* A função **tc_setup()** Configura o timer e os contadores, habilita o **TC2** para ser usado como interrupção de leitura do ADC;
* A função **ADC_Handler()** é a interrupção do ADC, nesta lê o ADC a cada pulso par to timer e seta o valor do ADC no DAC, em pulsos impares zera o valor do DAC sem ler o ADC, segundo Datasheet não é recomendado fazer leituras do ADC com frequências superiores a 1 MHz, pois o resultado é incerto da leitura;
* Assim obtendo a modulação AM, ou seja simula o processo de multiplicar uma portadora de onda quadrada pela mensagem usando a lógica do Arduíno Due;
* As frequências de operação suportadas do Arduíno estão entre 100Hz ate 807.44kHz, devido tanto o ADC quando o DAC terem limites da frequência de leitura e escrita, esta foi a máxima frequência possível de obter resultado.
* Como o radio utilizado só tem definição de seleção de frequência de 10 em 10kHz, foi escolhido um valor de timer que entregasse uma frequência terminada em zero, assim, a frequência escolhida foi de 700kHz pois é fica dentro da faixa AM, não é utilizada na cidade e o arduino consegue gerar com precisão.
* Referencia da configuração usada consulte o [Datasheet SAM3X / SAM3A Series](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf), neste tem uma descrição completa do que é cada registrador, função e parâmetro que foi neste usado com detalhes.

### Filtragem:

Como o objetivo é obter um sinal modulado em amplitude muito próximo do que é mostrado na imagem abaixo temos de fazer a filtragem.

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/am_mod.png)

O filtro tem algumas funções:

* Eliminar a componente CC do sinal
* Eliminar ou diminuir a contribuição de frequências adicionais que surgem no processo de modulação, deixando apenas passar a frequência fundamental da portadora 700 kHz
* Limitar a largura de banda do sinal
* Obter um sinal senoidal o mais próximo de uma AM comercial

Assim podemos ver o efeito que o filtro causa no sinal nas imagens abaixo, sem filtragem o sinal não é um AM puro, apos o filtro o sinal esta bem próximo de um AM comercial.

#### Sinal sem filtragem:

O canal 0 do osciloscópio (sinal amarelo) esta conectado na saída do DAC, oscilando a uma frequência de 700kHz.<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio (um tom senoidal 1kHz) com um offset, sinal visto pela porta do ADC.<br>
FFT(0) do osciloscópio (expectro amarelo) é o espectro FFT do sinal do DAC.<br>

![out_antes_fitro](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/fft_1kz_dac.png)

#### Sinal com filtragem:

O canal 0 do osciloscópio (sinal amarelo) esta conectado na saída do filtro, oscilando a uma frequência de 700kHz.<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio (um tom senoidal 1kHz) com um offset, sinal visto pela porta do ADC.<br>
FFT(0) do osciloscópio (expectro amarelo) é o espectro FFT do sinal do filtro RLC.<br>

![out_depois_filtro](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/fft_1kz_filtro.png)

Como é possível ver os picos no espectro de Fourier a frequancia 0, 200, 500 e 600 kHz são eliminados, o pico em 100 kHz diminui bastante, e a energia do sinal se concentra na frequência de operação 700 kHz.

### Escolha do filtro:

Neste foi proposto e testado dois filtros passa faixa, um RC e outro RLC, ambos funcionam porem o que demostra melhor resultado é o filtro RLC, pois este consegue entregar um sinal mais próximo de uma senoide quando comparado ao sinal do filtro RC, um sinal senoidal é o mais ideal, pois a potencia do sinal fica concentrada muito mais na frequência principal da portadora, assim evitando sinal fora da banda de operação e caso o sinal fosse amplificado evita amplificar sinais indesejados.<br>

Nas imagens abaixo temos uma visualização dos resultado de cada filtro

#### Zoom do sinal do DAC

![zoom_dac](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/zoom_dac.png) 

#### Zoom do DAC apos o Filtro RC

![zoom_filtro_rc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/zoom_filtro_rc.png) 

#### Zoom do DAC apos o Filtro RLC

![zoom_filtro_rlc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/zoom_filtro_rlc.png) 

#### FFT Filtro RC

![FFT_filtro_rc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/FFT_filtro_rc.png) 

#### FFT Filtro RLC

![FFT_filtro_rlc](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/FFT_filtro_rlc.png)

Como é possível ver o sinal do filtro RC não é uma senoide, mas o sinal do RLC é muito próximo de uma senoide. Pelo espectro da FFT vemos que no filtro RLC a energia se concentra próxima a frequência de operação 700 kHz, já no filtro RC temos um pico em 600 kHz e o pico em 100 kHz é bem maior.

### Amplificador:

Sem amplificador temos um leve ruido ao escutar o áudio em um radio (confira video demostrativo no final deste), assim foi adicionado este amplificador classe A, o mais simples amplificador possível, alimentado pelo próprio Arduíno e foi obtido sucesso em remover o ruido sem adicionar problemas, como outros ruídos e amplificar frequências indesejadas, o ganho de tensão ficou em 2,35 vezes.

Abaixo a comparação dos sinais com e sem amplificação, no canal 0 em amarelo o sinal amplificado, no canal 1 em vermelho sem amplificação.

#### Sinal visão geral:

![amp_geral](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/amp_geral.png)

#### Sinal com Zoom:

![amp_zoom](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/amp_zoom.png)

#### FFT saída do filtro:

![amp_zoom](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/FFT_filtro_rlc_amp.png)

### Imagens 3d circuito e PCB Arduíno:

[PCB veja este pdf](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/pcb.pdf)

![ci_3d](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci_3d.png)

![ci_3d_2](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci_3d_3.png)

![ci_3d_2](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci_3d_2.png)

### Imagens do circuito montado para test no Arduíno:

Com filtro RLC e amplificador

![CI_DONE1](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_4.jpg?raw=true)

![CI_DONE2](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_5.jpg?raw=true)


Com filtro RLC sem amplificador

![CI_DONE1](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_2.jpg?raw=true)

![CI_DONE2](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_3.jpg?raw=true)

Com filtro RC sem amplificador

![CI_DONE3](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_0.jpg?raw=true)

![CI_DONE4](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_1.jpg?raw=true)

## Apresentação

Como tudo funcionando, apresentemos algumas formas de onda e um video demostrativo do funcionamento.

### Resultados de alguns sinais de áudio:

Como é possível ver o sinal modulado acompanha perfeitamente o sinal da mensagem.

O canal 0 do osciloscópio (sinal amarelo) esta conectado na saída do filtro RLC, oscilando a uma frequência de 700kHz (frequência de operação pode variar um pouco nas imagens a seguir, isto é devido o osciloscópio por termos sinais com frequências tão longe, acaba calculando um pouco fora, confira as imagens de zoom anterior para ver que a portadora esta oscilando a 700 kHz fixo).<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio a mensagem, com um offset visto pela porta do ADC.<br>

1kHz https://www.youtube.com/watch?v=3FBijeNg_Gs

![1k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/tone_1_kHz.png)

3kHz https://www.youtube.com/watch?v=CAwpIiDQv5w

![3k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/tone_3_kHz.png)

7kHz https://www.youtube.com/watch?v=3_ciJKxe9VU

![7k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/tone_7_kHz.png)

1kHz + 3kHz + 7kHz ... todos habilitados ao mesmo tempo no notebook

![soma](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/soma_tom_1_3_7_kHz.png)

### Video demostrativos do projeto funcionando num rádio

No video abaixo uma breve explicação do funcionamento e como é possível ver a musica fica com uma qualidade de áudio muito boa para AM.

[Video demostrativos filtro RLC com amplificação](https://youtu.be/n_dgm5joqQQ)


Os vídeos abaixo são os passos anteriores de testes que levaram ao video final que é o acima.

[Video demostrativos filtro RLC sem amplificação](https://youtu.be/qsMYSg_q5ks)

[Video demostrativos filtro RC sem amplificação](https://youtu.be/yYcabqJUBEA)

## Conclusão

Aqui fazer uma conclusão.
