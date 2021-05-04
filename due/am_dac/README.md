# Transmissor AM usando Arduíno projeto em andamento da faculdade

## Introdução:
Neste usamos um Arduíno DUE

![CI](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci.png)

Este transmissor AM digital funciona como qualquer outro transmissor AM comercial digital

Em uma modulação AM temos a imagem abaixo, o sinal modulado varia sua amplitude em relação ao sinal modulador ou sinal de áudio a modular.

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/am_mod.png)

Neste projeto para modular o sinal foi usado esta mesma técnica mas a portadora é uma onda quadrada, assim o resultado do sinal modulado é o visto na imagem abaixo, sendo o sinal de mensagem uma onda senoidal que oscila entre 0 e um valor positivo de tensão

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/modulado.png)

## Explicação detalhada funcionamento do processo lógico:

Assim para gerar esta modulação no microcontrolador temos os seguintes passos [Configuração do Arduíno esta no arquivo am.ino](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/am_dac.ino)

* fazer a explicação detalhada é bem simple seta o ADC, DAC e timer, lê o ADC a cada pulso par to timer e seta no DAC em pulsos impares zera o dac, fazendo assim a modulação

O resultado deste processo pode ser visto na pratica na imagem abaixo:

Temos [um sinal de áudio de um tom senoidal 1kHz](https://www.youtube.com/watch?v=3FBijeNg_Gs) na entrada do circuito.

O canal 0 do osciloscópio (sinal amarelo) sua amplitude é de 3.3V esta conectado na porta do DAC, oscilando a uma frequência de 700kHz.<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio original com um offset visto pela porta do ADC.<br>

Como é possível ver o sinal modulado acompanha perfeitamente o sinal da mensagem.

![1k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ton_1_kHz.png)

## Explicação detalhada funcionamento do processo em termos de circuito:

![CI](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ci.png)

O funcionamento do circuito ao redor do Arduíno é muito simples.

* Na entrada de áudio foi conectada a saída de áudio de um notebook, este sinal segue ate o capacitor
* O capacitor de 10uF funciona apenas como um filtro bloqueando sinal CC deixando passar apenas o sinal de áudio analógico vindo do entrada de áudio.
* Os dois resistores de 10kΩ formão um divisor de tensão, divide a tensão referencia do ADC pela metade
* O sinal de áudio esta oscilando sobre 0V, ou seja, para mais e para menos de 0V com um pico de amplitude de 0,5V (Ajustado através do volume da fonte de audio), quando este é conectado ao capacitor duas coisas ocorrem, a parte CC do sinal é filtrada e o sinal é elevado em tensão (um offset para mais) para agora oscilar abaixo e acima de **VREF 3.3V/2 = 1,65V**, assim temos o resultado do sinal visto na imagem do osciloscópio
* Assim este sinal filtrado e elevado é conectado na porta **A0** dos ADC
* Temos um timer configurado a 1.4 MHz
* O Arduíno então a cada pulso impar do timer zera o DAC, e pulsos pares lê o valor do ADC e seta este valor no DAC
* E na porta **DAC1** temos a saída o sinal modulado AM

# Resultado:

Temos assim a imagem do circuito montado no Arduíno:

![CI_DONE](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_desc.jpg?raw=true)

![CI_DONE](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_0.jpg?raw=true)

![CI_DONE](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_1.jpg?raw=true)

![CI_DONE](https://github.com/fgl27/Arduino_samples/blob/master/due/am_dac/arduino_montado_2.jpg?raw=true)

Frequências suportadas:

Varia de 1kHz ate 807.44kHz, devido tanto o ADC quando o DAC terem limites da frequência de leitura e escrita, esta foi a máxima frequência possível.

Resultados de alguns sinais de áudio:

O canal 0 do osciloscópio (sinal amarelo) sua amplitude é de 3.3V esta conectado na porta do DAC, oscilando a uma frequência de 700kHz.<br>
O canal 1 do osciloscópio (sinal vermelho) é o sinal de áudio original com um offset visto pela porta do ADC.<br>

1kHz https://www.youtube.com/watch?v=3FBijeNg_Gs

![1k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ton_1_kHz.png)

1kHz zoom, para ver como os pulsos ficam bem definidos

![1k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ton_1_kHz_zoom.png)

3kHz https://www.youtube.com/watch?v=CAwpIiDQv5w

![3k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ton_3_kHz.png)

7kHz https://www.youtube.com/watch?v=3_ciJKxe9VU

![7k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/ton_7_kHz.png)

1kHz + 3kHz + 7kHz ... todos habilitados ao mesmo tempo no notebook

![soma](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/due/am_dac/soma_tom_1_3_7_kHz.png)

# Video demostrativos do projeto funcionando num rádio

No video abaixo uma breve explicação do funcionamento e como é possível ver a musica fica com uma qualidade de áudio muito boa para AM.

https://drive.google.com/file/d/1mpU2m4KYqXoMfls-M0ac0URaO17hGFsG/view?usp=sharing

