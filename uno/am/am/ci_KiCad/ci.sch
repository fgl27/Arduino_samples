EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_UNO_R3 A1
U 1 1 6053856C
P 5950 3800
F 0 "A1" H 5650 5000 50  0001 C CNN
F 1 "Arduino_UNO_R3" H 5950 5400 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 5950 3800 50  0001 C CIN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 5950 3800 50  0001 C CNN
	1    5950 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:Antenna A1
U 1 1 605397BD
P 4500 3900
F 0 "A1" H 4450 4250 50  0000 L CNN
F 1 "Antena" H 4400 4100 50  0000 L CNN
F 2 "" H 4500 3900 50  0001 C CNN
F 3 "~" H 4500 3900 50  0001 C CNN
	1    4500 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 6053A6BF
P 6950 3550
F 0 "R1" H 7020 3596 50  0000 L CNN
F 1 "10k Ω" H 7020 3505 50  0000 L CNN
F 2 "" V 6880 3550 50  0001 C CNN
F 3 "~" H 6950 3550 50  0001 C CNN
	1    6950 3550
	1    0    0    -1  
$EndComp
$Comp
L pspice:CAP C1
U 1 1 6053AEF6
P 7650 3800
F 0 "C1" V 7965 3800 50  0000 C CNN
F 1 "10 μF" V 7874 3800 50  0000 C CNN
F 2 "" H 7650 3800 50  0001 C CNN
F 3 "~" H 7650 3800 50  0001 C CNN
	1    7650 3800
	0    -1   -1   0   
$EndComp
$Comp
L Connector:AudioJack2 J?
U 1 1 6053CCAD
P 8250 3800
F 0 "J?" V 8328 3670 50  0001 R CNN
F 1 "Entrada do sinal de áudio" H 8000 3800 50  0000 R CNN
F 2 "" H 8250 3800 50  0001 C CNN
F 3 "~" H 8250 3800 50  0001 C CNN
	1    8250 3800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 6053E3D1
P 6950 4000
F 0 "R2" H 7020 4046 50  0000 L CNN
F 1 "10k Ω" H 7020 3955 50  0000 L CNN
F 2 "" V 6880 4000 50  0001 C CNN
F 3 "~" H 6950 4000 50  0001 C CNN
	1    6950 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+5VP #PWR?
U 1 1 6054325A
P 5850 2600
F 0 "#PWR?" H 5850 2450 50  0001 C CNN
F 1 "+5VP" H 5865 2773 50  0000 C CNN
F 2 "" H 5850 2600 50  0001 C CNN
F 3 "" H 5850 2600 50  0001 C CNN
	1    5850 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 2800 5850 2600
Wire Wire Line
	6950 3700 6950 3800
Connection ~ 6950 3800
Wire Wire Line
	6950 3800 6950 3850
Wire Wire Line
	8050 3800 7900 3800
$Comp
L power:GNDREF GND
U 1 1 605473BE
P 8050 4100
F 0 "GND" H 8000 3900 50  0000 L CNN
F 1 "GNDREF" H 8055 3927 50  0001 C CNN
F 2 "" H 8050 4100 50  0001 C CNN
F 3 "" H 8050 4100 50  0001 C CNN
	1    8050 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 4100 8050 3900
Wire Wire Line
	6450 3800 6950 3800
Wire Wire Line
	6450 3600 6700 3600
Wire Wire Line
	6700 3600 6700 3400
Wire Wire Line
	6700 3400 6950 3400
Wire Wire Line
	6950 3800 7400 3800
$Comp
L power:GNDREF GND
U 1 1 60554C55
P 6950 4400
F 0 "GND" H 6900 4200 50  0000 L CNN
F 1 "GNDREF" H 6955 4227 50  0001 C CNN
F 2 "" H 6950 4400 50  0001 C CNN
F 3 "" H 6950 4400 50  0001 C CNN
	1    6950 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 4150 6950 4400
$Comp
L power:GNDREF GND
U 1 1 60558CB3
P 6050 5100
F 0 "GND" H 6000 4900 50  0000 L CNN
F 1 "GNDREF" H 6055 4927 50  0001 C CNN
F 2 "" H 6050 5100 50  0001 C CNN
F 3 "" H 6050 5100 50  0001 C CNN
	1    6050 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 5100 6050 4900
$Comp
L pspice:CAP C3
U 1 1 60911131
P 4500 4650
F 0 "C3" H 4815 4650 50  0000 C CNN
F 1 "447 pF" H 4850 4550 50  0000 C CNN
F 2 "" H 4500 4650 50  0001 C CNN
F 3 "~" H 4500 4650 50  0001 C CNN
	1    4500 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF GND
U 1 1 609123E8
P 4500 5150
F 0 "GND" H 4450 4950 50  0000 L CNN
F 1 "GNDREF" H 4505 4977 50  0001 C CNN
F 2 "" H 4500 5150 50  0001 C CNN
F 3 "" H 4500 5150 50  0001 C CNN
	1    4500 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 4900 4500 5150
$Comp
L Device:R_POT POT1
U 1 1 60921746
P 5050 4300
F 0 "POT1" V 4935 4300 50  0000 C CNN
F 1 "1k Ω" V 4844 4300 50  0000 C CNN
F 2 "" H 5050 4300 50  0001 C CNN
F 3 "~" H 5050 4300 50  0001 C CNN
	1    5050 4300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5200 4300 5450 4300
Wire Wire Line
	4900 4300 4850 4300
Wire Wire Line
	4500 4100 4500 4300
Connection ~ 4500 4300
Wire Wire Line
	5050 4150 4850 4150
Wire Wire Line
	4850 4150 4850 4300
Connection ~ 4850 4300
Wire Wire Line
	4850 4300 4500 4300
Wire Wire Line
	4500 4300 4500 4400
Text Notes 3400 3400 0    50   ~ 0
Saída antena e um filtro passa baixa ajustavel\nFrequência de corte 800kHz R = 445,06 Ω
$EndSCHEMATC
