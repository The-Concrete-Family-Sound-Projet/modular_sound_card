#include "m_pd.h"
#include <math.h>

static t_class *sigtoint_class;

typedef struct _sigtoint {
	
	t_object	x_obj;
	int		x_n;
	t_outlet *f_out;
	t_float		x_f;
	
} t_sigtoint;

static t_int *sigtoint_perform(t_int *w){
	
	t_sigtoint *x	= (t_sigtoint *)(w[1]);
	t_sample *in		= (t_sample *)(w[2]);
	int n 			= (int)(w[3]);
	int neg_val = 0;
	t_float twoscomp;
	
	while (n--){ 
		if(in[n] < 0){
			neg_val = round(32767 * -in[n]);
			twoscomp = (float)((neg_val ^ 65535) +1);
		}
		else
			twoscomp = (float)round(32767 * in[n]);
	}
	outlet_float(x->f_out, twoscomp);
	return (w+4);
}

void sigtoint_dsp(t_sigtoint *x, t_signal **sp){
	
	dsp_add(sigtoint_perform, 3,
		x, sp[0]->s_vec, sp[0]->s_n);
}

static void *sigtoint_new(t_floatarg f){
	
	t_sigtoint *x = (t_sigtoint *)pd_new(sigtoint_class);
	x->x_n = f;
	x->x_f = 0;
	x->f_out = outlet_new(&x->x_obj, &s_float);
	
	return x;
}

void sigtoint_setup(void){
	
	sigtoint_class = class_new(gensym("sigtoint"),
					(t_newmethod)sigtoint_new,
					0,
					sizeof(t_sigtoint),
					CLASS_DEFAULT,
					A_DEFFLOAT,
					0);
									
	class_addmethod(sigtoint_class, 
			(t_method)sigtoint_dsp, 
			gensym("dsp"), 
			0);
	CLASS_MAINSIGNALIN(sigtoint_class, t_sigtoint, x_f);

	class_sethelpsymbol(sigtoint_class, gensym("sigtoint"));
}
