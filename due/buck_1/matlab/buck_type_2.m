
%rmpath('/home/fgl27/MATLAB/R2018b/toolbox/maple')
%rmpath('/home/fgl27/MATLAB/R2018b/toolbox/maple/util')
%clear('classes');

clear % clear workspace
clc % clear cmd window
format ShortEng;%prevent to display a separated exx multiplayer example (1.00*e19 * 0.5555)

vd = 12;
vo = 6;
imax = 25;
vpwm = 12;
rl = 10*10^-3;
L = 33*10^-6;
rc = 10*10^-3;
C = 2200*10^-6;
R = 200*10^-3;
F = 200*10^3;
Fc= F/10;
R1 = 10*10^3;

wo = 1/((L*C)^(1/2));
E = (((1/(C*R)) + ((rc+rl)/L)))/(2*wo);
wz = 1/(rc*C);

s=i*2*pi*Fc;

Tp = (vd*((wo^2)/wz)*(s + wz))/(s^2 + 2*E*wo*s + wo^2);
Tm = 1/vpwm;
T1 = Tp*Tm;

ANG = (atand(imag(T1)/real(T1)) -180);
G1 = abs(T1);

s = tf('s');
Tp = (vd*((wo^2)/wz)*(s + wz))/(s^2 + 2*E*wo*s + wo^2);
Tm = 1/vpwm;
T1 = Tp*Tm;

[Gm,Pm,Wcg,Wcp] = margin(T1);

boost = 60 - ANG - 90; % 60 can be 45 to 60, or using the Pm value from margin
Wc = Fc*2*pi;
K = abs(tand(45 + boost/2));
C2 = G1/(K*R1*Wc);
C1 = C2*(K^2 + 1);
R2 = K/(C1*Wc);

sdisp = @(x) strtrim(evalc(sprintf('disp(%g)', x)));
fprintf('Fc		= %s \n', sdisp(Fc)');
fprintf('Wc		= %s \n', sdisp(Wc)');
fprintf('G1		= %s \n', sdisp(G1)');
fprintf('ANG		= %s \n', sdisp(ANG)');
fprintf('K		= %s \n', sdisp(K)');
fprintf('R1		= %s \x03A9\n', sdisp(R1)');
fprintf('R2		= %s \x03A9\n', sdisp(R2)');
fprintf('C1		= %s F\n', sdisp(C1)');
fprintf('C2		= %s F\n', sdisp(C2)');


Polo = (C1+C2)/(R2*C1*C2);
Zero = 1/(R2*C1);
Gain = 1/(R1*C2);

fprintf('Zero		= %s F\n', sdisp(Zero)');
fprintf('Polo		= %s F\n', sdisp(Polo)');
fprintf('Gain		= %s F\n', sdisp(Gain)');

TC= Gain*(s + Zero)/(s*(s + Polo))
Tol=T1*TC;

 options = bodeoptions;
 options.FreqUnits = 'Hz';
%options.Xlim = [100,100000];%Lim freq
%options.Ylim{1} = [-100,100];%Lim Ganho
%options.Ylim{2} = [-240,10];%lim Phase
hold on;
 bode(T1,options);
 bode(TC,options);
 bode(Tol,options);
 legend({'T1', 'Tc', 'Tol'});
 hold off;
 grid on

C = c2d(TC,1/F,'tustin')
[num,den] = tfdata(C);

fprintf('//INÍCIO do CÓDIGO\n\n');
fprintf('//CONTROLE NO MODO TENSAO - Conversor Buck\n');
fprintf('//Passo de calculo da simulaçao = %ds\n', 1/(F*100));
fprintf('//Frequencia de amostragem = Frequencia de chaveamento = %dHz\n', F);
fprintf('#define N_PONTOS 100\n');
fprintf('static int cpt = 0;\n');
fprintf('static int d = 50;\n');
fprintf('static float uk = 0, uk_1 = 0, uk_2 = 0, xk = 0, xk_1 = 0, xk_2 = 0;\n\n');
fprintf('static float Vref = %d;\n\n', vo);
fprintf('static float Imax = %d;\n',imax);
fprintf('static float Irefoffset = %f;//Imax/1000 = *0.02 minor offset to keep I below max threshold\n',imax/1000);
fprintf('static float Iref = 0;\n\n');
fprintf('static float Vout = 0;\n');
fprintf('static float Iout = 0;\n\n');
fprintf('//Atualiza o sinal de comando Vcmd\n');
fprintf('if(cpt<=d) out[0] = 1;\n');
fprintf('else       out[0] = 0;\n\n');
fprintf('if(cpt == N_PONTOS)\n');
fprintf('   {\n');
fprintf('      //Executa a amostragem e roda o algoritmo de controle\n');
fprintf('      cpt = 0;\n');
fprintf('      Vout = in[0];\n');
fprintf('      Iout = in[1];\n\n');
fprintf('      Iref = Imax - (Imax*Irefoffset);\n\n');
fprintf('      if (Iref <= Iout) xk = Iref - Iout;\n');
fprintf('      else xk = Vref - Vout;\n\n');
fprintf('      uk = ');
addplus = 0;
for i = 1:1:length(num{1})
    if num{1}(i) ~= 0
        if addplus == 0
            fprintf(' %.10f*xk', num{1}(i)');
        else
            if num{1}(i) > 0
               fprintf(' + %.10f*xk_%d', num{1}(i),i-1');
            else
               fprintf(' - %.10f*xk_%d', abs(num{1}(i)),i-1');
            end
        end
    addplus = 1;
    end
end

for i = 2:1:length(den{1})
    if den{1}(i) ~= 0
        if addplus == 0
            fprintf(' %.10f*uk', (-1)*den{1}(i)');
        else
            if den{1}(i) > 0
               fprintf(' - %.10f*uk_%d', den{1}(i),i-1');
            else
               fprintf(' + %.10f*uk_%d', abs(den{1}(i)),i-1');
            end
        end
    addplus = 1;
    end
end
fprintf(';\n\n');
fprintf('      d = uk*%.1f;\n', 100/vpwm');
fprintf('      if (d > 100)    d = 100;\n');
fprintf('      if(d < 0)       d = 0;\n');
fprintf('      xk_2 = xk_1;\n');
fprintf('      xk_1 = xk;\n');
fprintf('      uk_2 = uk_1;\n');
fprintf('      uk_1 = uk;\n');
fprintf('      //Conclui a operaçao de controle, e inicia outro periodo de chaveamento, com novo valor para d\n');
fprintf('    }\n');
fprintf('cpt++;\n');
fprintf('out[1] = d;\n\n');
fprintf('//FIM do CÓDIGO\n');