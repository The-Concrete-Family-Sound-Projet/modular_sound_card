#include "m_pd.h"

static t_class *through_tilde_class;

typedef struct _through_tilde {
	
	t_object	x_obj;
	
	t_float		f;
	
	t_outlet	*out_sig;
	
} t_through_tilde;

t_int *through_tilde_perform(t_int *w){
	
	t_through_tilde *x	= (t_through_tilde *)(w[1]);
	t_sample *in		= (t_sample *)(w[2]);
	t_sample *out 		= (t_sample *)(w[3]);
	int n 			= (int)(w[4]);
	
	while (n--) *out++ =  *in++;
	
	return (w+5);
}

void through_tilde_dsp(t_through_tilde *x, t_signal **sp){
	
	dsp_add(through_tilde_perform, 4, x,
		sp[0]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

void through_tilde_free(t_through_tilde *x){
	
	outlet_free(x->out_sig);
}

void *through_tilde_new(void){
	
	t_through_tilde *x = (t_through_tilde *)pd_new(through_tilde_class);
	
	x->out_sig = outlet_new(&x->x_obj, &s_signal);
	
	return (void *)x;
}

void through_tilde_setup(void){
	
	through_tilde_class = class_new(gensym("through~"),
					(t_newmethod)through_tilde_new,
					(t_method)through_tilde_free,
					0,
					sizeof(t_through_tilde),
					CLASS_DEFAULT,
					A_DEFFLOAT,
					0);
									
	class_addmethod(through_tilde_class, 
			(t_method)through_tilde_dsp, 
			gensym("dsp"), 
			A_CANT,
			0);
	CLASS_MAINSIGNALIN(through_tilde_class, t_through_tilde, f);

	class_sethelpsymbol(through_tilde_class, gensym("through~"));
}
