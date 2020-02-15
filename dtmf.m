clear, clc, close all

fs=8000;
f_tonos=[697, 770, 852, 941,1209,1336,1477,1633];
f_tono=f_tonos(1);
n=0:1/fs:1;
signal= 0.5 * (sin(2*pi*f_tonos(2)*n) + sin(2*pi*f_tonos(7)* n));

figure, stem(signal)

N=205;

fft_signal= fft(signal(1:N));

freqs_fft= linspace(0,fs-fs/N, N);
figure, plot(freqs_fft, abs(fft_signal));

delay_line=zeros(8,2);
a_coeffs=[];
for i=1:8
[~,k_coeffs(i)]=min(abs(f_tonos(i)-freqs_fft));    
a_coeffs=[a_coeffs; 1 2*cos(2*pi*(k_coeffs(i)-1)/N) -1];
end    


for n=1:N
    signal_sample= signal(n);
    acc=0;
    for i=1:8
               
            acc=signal_sample*a_coeffs(i,1);
            acc=acc+ delay_line(i,1)*a_coeffs(i,2);
            acc=acc+ delay_line(i,2)*a_coeffs(i,3);
          
            delay_line(i,2)=delay_line(i,1);
            delay_line(i,1)=acc;
    end    
    
end


for i=1:8
  
    POWER(i)= delay_line(i,1)^2 + delay_line(i,2)^2 + delay_line(i,1)*delay_line(i,2)*2*a_coeffs(i,2);
    
end    






%% 
disp('k coeffs: ')
disp(k_coeffs)
disp('Frequency error for every position (Hz): ')
disp(error)

scale=2;
a_coeffs_short= num2str(round(a_coeffs/scale * 2^15))
b_coeffs_real_short= num2str(round(real(b_coeffs)/scale * 2^15))
b_coeffs_imag_short= num2str(round(imag(b_coeffs)/scale* 2^15))

format SHORTG
signal= round(signal*2^15).';
dlmwrite('test.dat', signal, ' ' )