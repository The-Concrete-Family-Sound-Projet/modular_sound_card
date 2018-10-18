#include "m_pd.h"

static t_class *through_tilde_class;

typedef struct _through_tilde {
	
	t_object	x_obj;
	int		x_n;
	t_float		x_f;
	
} t_through_tilde;

static t_int *through_tilde_perform(t_int *w){
	
	t_sample *in		= (t_sample *)(w[1]);
	t_sample *out 		= (t_sample *)(w[2]);
	int n 			= (int)(w[3]);
	
	while (n--) *out++ =  *in++;
	
	return (w+4);
}

void through_tilde_dsp(t_through_tilde *x, t_signal **sp){
	
	dsp_add(through_tilde_perform, 3,
		sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

static void *through_tilde_new(t_floatarg f){
	
	t_through_tilde *x = (t_through_tilde *)pd_new(through_tilde_class);
	x->x_n = f;
	x->x_f = 0;
	outlet_new(&x->x_obj, gensym("signal"));
	
	return x;
}

void through_tilde_setup(void){
	
	through_tilde_class = class_new(gensym("through~"),
					(t_newmethod)through_tilde_new,
					0,
					sizeof(t_through_tilde),
					CLASS_DEFAULT,
					A_DEFFLOAT,
					0);
									
	class_addmethod(through_tilde_class, 
			(t_method)through_tilde_dsp, 
			gensym("dsp"), 
			0);
	CLASS_MAINSIGNALIN(through_tilde_class, t_through_tilde, x_f);

	class_sethelpsymbol(through_tilde_class, gensym("through~"));
}
