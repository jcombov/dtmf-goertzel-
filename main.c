//////////////////////////////////////////////////////////////////////////////
// Template 1
// *  To use with probe points and graphs
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "DSPlib.h"
#include "iir_filter.h"

#define fdii_alpha 8
#define scale 1
#define N_samples 205

short inp_buffer;
short out_buffer;
short aux_test;
short Delays[8][N - 1];
short iir_a_coeffs[8][N];
short mean_power[8];
void IIR_DFII(short data, int filter);
short feedforward_phase(int filter);
int max_position(int ini_pos);
short dato;
short fichero[6129];

void main(void) {
	short *input = &inp_buffer;
	short *output = &out_buffer;
	int i, j, n;
	int location[2];

	for (i = 0; i < 8; i++) {
		fltoq15(iir_a[i], iir_a_coeffs[i], N);

	}

	for(i=0;i<6128;i++)
	{
		fichero[i]= *input;
	}

	int count=0;

	while (1) {
		if(count>6128)count=0;
		dato = fichero[count++];
		if (fabs(dato) > 8192) {

			for (i = 0; i < 8; i++) {
				for (j = 0; j < N - 1; j++) {
					Delays[i][j] = 0;
				}

			}

			for (n = 0; n < N_samples; n++) {

				dato = fichero[count++];

				for (i = 0; i < 8; i++) {

					IIR_DFII(dato, i);

				}
			}

			for (i = 0; i < 8; i++) {
				mean_power[i] = feedforward_phase(i);

				fprintf(stdout, "power output of filter %d:   %d\n", i + 1,
						mean_power[i]);
			}

			location[0] = max_position(0) - 1;
			location[1] = max_position(4) - 1;

			fprintf(stdout, "\n\nTECLA: %c \n\n",
					teclado[location[0]][location[1]]);

			for (n = 0; n < 300; n++) {
				dato = fichero[count++]; //breakpoint
			}
		}
	}
}

#pragma FUNC_CANNOT_INLINE(IIR_DFII);

void IIR_DFII(short data, int filter) {

	short *pDelays = &Delays[filter][0];
	short *pCoeff = &iir_a_coeffs[filter][1];
	short *pDelaysaux;
	long acc = 0;
	int j, i;

	acc = (long) data << (15 - fdii_alpha);

	acc += (long) *pCoeff++ * *pDelays++;
	acc += (long) *pCoeff++ * *pDelays++;

	pDelaysaux = pDelays-- - 2;
	*pDelays-- = *pDelaysaux--;

	*pDelays = (short) (acc >> 15 - scale);

}

#pragma FUNC_CANNOT_INLINE(num_mean);

short feedforward_phase(int filter) {
	long acc = 0;
	short salida, aux2;
	short Delay0 = Delays[filter][0];
	short Delay1 = Delays[filter][1];
	short coeff_k = iir_a_coeffs[filter][1];

	acc = (long) Delay0 * Delay0;
	acc += (long) Delay1 * Delay1;
	aux2 = ((long) Delay0 * Delay1) >> 15;
	acc -= (long) coeff_k * aux2;

	salida = (short) (acc >> 15);

	return salida;
}

int max_position(int ini_pos) {

	int location, c;
	short maximum;

	maximum = mean_power[ini_pos];
	location = 1;

	for (c = 1; c < 4; c++) {
		if (mean_power[c + ini_pos] > maximum) {
			maximum = mean_power[c + ini_pos];
			location = c + 1;
		}
	}

	return location;
}
