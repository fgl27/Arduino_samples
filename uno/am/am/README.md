# Transmissor AM usando Arduíno projeto em andamento da faculdade

# Funcionamento explicação:

## Introdução:
Neste usamos um Arduíno Uno Versão R3 (qualquer versão deve funcionar da mesma forma) para fazer um simples mas funcional transmissor AM didital

![CI](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/ci.png)

Este transmissor AM digital funciona no mesmo principio de um transmissor analógico mas para visualizar a amplitude é preciso entender o processo de modulação aplicada.

Em uma modulação AM temos a imagem abaixo, o sinal modulado varia sua amplitude em relação ao sinal modulador ou sinal de áudio a modular.

![am_mod](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/am_mod.png)

Como o PWM deste microcontrolador só tem dois valores de sinal alto e baixo não é possível modificar sua amplitude, assim usamos um processo similar ao da modulação PWM bipolar, que é usada por exemplo para controlar chaveamento de ponte H usada em conversores CC AC.

![MOD_BIP](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/mod_bip.png)

Fonte, material da aula Leitura_11 da cadeira Eletrônica de Potencia II, professor Cláudio, ministrada em 2020 semestre online.

## Explicação detalhada funcionamento do processo lógico:

Na modulação PWM bipolar temos o sinal modulado entre +VCC e -VCC neste caso temos o sinal modulado apenas entre +VCC e GND, porem, o principio é o mesmo.

Assim para gerar esta modulação no microcontrolador temos os seguintes passos [Configuração do Arduíno esta no arquivo am.ino](https://github.com/fgl27/Arduino_samples/blob/master/uno/am/am/am.ino)

* Na função **void setup()** é feita a preparação, é habilitado o PWM para oscilar em 800kHz, é habilitado o ADC para operar em uma amostragem de 19,2kHz, é preparado duas interrupções e é setado a referencia do ADC para 1.1V
* Apos o processo de inicialização a cada pulso de leitura do ADC a função de interrupção **ISR(TIMER1_OVF_vect)** é executada, nesta setamos o sinal de PWM para sinal alto e é feita uma leitura do valor do ADC atual, com base na comparação deste valor é determinado o quanto tempo este sinal do PWM vai ficar alto, assim o valor lido do ADC é usado nesta comparação para determinar o tempo da próxima interrupção da função de interrupção **ISR(TIMER1_COMPA_vect)**
* Quando a função **ISR(TIMER1_COMPA_vect)** é executada o sinal do PWM é setado para sinal baixo
* E assim por diante se repete... Ao variar o sinal de áudio na entrada do ADC varia o sinal do PWM gerando assim uma modulação PWM bipolar

O resultado deste processo pode ser visto na pratica na imagem abaixo:

Temos [um sinal de áudio de um tom senoidal 1kHz](https://www.youtube.com/watch?v=3FBijeNg_Gs) na entrada do circuito.

O canal 0 do osciloscópio (sinal amarelo) sua amplitude é de 5V esta conectado na porta do PWM.<br>
O canal 1 do osciloscópio (sinal verde) com amplitude de 1V esta conectado na porta do ADC.

Como é possível ver temos um leve atraso entre o sinal de entrada e o de saída, este é devido ao tempo que o Arduíno leva para processar o sinal.

![1k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/ton_1_kHz.png)

## Explicação detalhada funcionamento do processo em termos de circuito:

![CI](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/ci.png)

O funcionamento do circuito ao redor do Arduíno é muito simples.

* Na entrada de áudio foi conectada a saída de áudio de um notebook, este sinal segue ate o capacitor
* O capacitor de 10uF funciona apenas como um filtro bloqueando sinal CC deixando passar apenas o sinal de áudio analógico vindo do entrada de áudio.
* Os dois resistores de 10kΩ formão um divisor de tensão, divide a tensão referencia do ADC pela metade
* O sinal de áudio esta oscilando sobre 0V, ou seja, para mais e para menos de 0V com um pico de amplitude de 0,5V (Ajustado através do volume da fonte de audio), quando este é conectado ao capacitor duas coisas ocorrem, a parte CC do sinal é filtrada e o sinal é elevado em tensão (um offset para mais) para agora oscilar abaixo e acima de **AREF/2 = 0,55V**, assim temos o resultado do sinal visto na imagem do osciloscópio
* Assim este sinal filtrado e elevado é conectado na porta **A0** dos ADC
* O Arduíno então faz a comparação explicada no processo lógico
* E na porta **D11** temos a saída do sinal do PWM, este é conectado a antena

## Modelando usando um circuito:

É possível verificar este principio em um simulador com um circuito simples não controlado, abaixo temos como ficaria este circuito:

![CI](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/ci_model.png)

O processo de funcionamento deste é o mesmo do microcontrolado, porem visualizando este pode ser mais facil para alguns de entender o processo.
Assim na imagem acima temos, um sinal de áudio sendo filtrado e elevado, apos temos um comparador a 19.2kHz simulando o adc, e apos temos um sinal PWM e uma porta logica AND simulando assim o processo de comparação, no lado direito foi colocado um filtro de 800kHz para verificar como fica o sinal em um receptor, este sinal não é o sinal de audio reconstruído pois este processo é um tanto complexo para simular, este sinal é apenas para vermos a modulação filtrada.

O resultado gráfico da saída deste circuito abaixo, como podemos ver é muito próximo do sinal que temos no osciloscópio feito na pratica


![sem_zoom](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/sem_zoom.png)

Aproximando mostrando o sinal, desde o pico máximo ate o mínimo.

![com_zoom](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/com_zoom.png)

# Resultado:

Temos assim a imagem do circuito montado no Arduíno:

![CI_DONE](https://github.com/fgl27/Arduino_samples/blob/master/uno/am/am/arduino_montado_2_dec.jpg?raw=true)

![CI_DONE](https://github.com/fgl27/Arduino_samples/blob/master/uno/am/am/arduino_montado_0.jpg?raw=true)

![CI_DONE](https://github.com/fgl27/Arduino_samples/blob/master/uno/am/am/arduino_montado_1.jpg?raw=true)

![CI_DONE](https://github.com/fgl27/Arduino_samples/blob/master/uno/am/am/arduino_montado_2.jpg?raw=true)

Frequências suportadas:

![freq_possiveis](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/freq_possiveis.png)

Resultados de alguns sinais de áudio:

O canal 0 do osciloscópio (sinal amarelo) sua amplitude é de 5V esta conectado na porta do PWM.<br>
O canal 1 do osciloscópio (sinal verde) com amplitude de 1V esta conectado na porta do ADC.

1kHz https://www.youtube.com/watch?v=3FBijeNg_Gs

![1k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/ton_1_kHz.png)

3kHz https://www.youtube.com/watch?v=CAwpIiDQv5w

![3k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/ton_3_kHz.png)

7kHz https://www.youtube.com/watch?v=3_ciJKxe9VU

![7k](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/ton_7_kHz.png)

1kHz + 3kHz + 7kHz ... todos habilitados ao mesmo tempo no notebook

![soma](https://raw.githubusercontent.com/fgl27/Arduino_samples/master/uno/am/am/soma_tom_1_3_7_kHz.png)

## Video demostrativos do projeto funcionando num rádio

### Funcionamento geral, tons e musica

https://drive.google.com/file/d/1okv3rmkzH98PiDdSIJo7hSuNKqV3mdVJ/view?usp=sharing

## Teste de Banda

https://drive.google.com/file/d/1r-HV3g7FyxEeqyj1_8ohvYdK9GOixzTP/view?usp=sharing

## Referencias usadas

https://www.youtube.com/watch?v=Xy7DiPVi18Y

https://www.instructables.com/AM-Transmitter-With-Arduíno/

https://www.youtube.com/watch?v=gJeY2zaw7q0
