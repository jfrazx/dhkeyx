/*

Copyright (c) 2007, Simon Menke 
All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met: 

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution. 
 * All advertising materials mentioning features or use of this software 
   must display the following acknowledgement: This product includes 
   software developed by 3motions and its contributors. 
 * Neither the name of 3motions nor the names of its contributors may be 
   used to endorse or promote products derived from this software without 
   specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 

 */

#include "ruby.h"
#include <openssl/bn.h>

static VALUE rb_cDHKey;

struct dh_DHKey {
	BN_CTX * ctx;
	
	BIGNUM * base;
	BIGNUM * prime;
	BIGNUM * private_key;
	BIGNUM * my_public_key;
	BIGNUM * their_public_key;
	BIGNUM * shared_key;
};

static VALUE rb_DH_key_new(VALUE self, VALUE args);
static VALUE rb_DH_key_initialize(VALUE self, VALUE args);
#define def_c_get_attr(n) static VALUE rb_DH_key_get_ ## n (VALUE self);
	def_c_get_attr(base)
	def_c_get_attr(prime)
	def_c_get_attr(private_key)
	def_c_get_attr(my_public_key)
	def_c_get_attr(their_public_key)
	def_c_get_attr(shared_key)
	
#define def_c_set_attr(n) static VALUE rb_DH_key_set_ ## n (VALUE self, VALUE v);
	def_c_set_attr(base)
	def_c_set_attr(prime)
	def_c_set_attr(private_key)
	def_c_set_attr(their_public_key)
void dh_DHKey_free(struct dh_DHKey* key);


// redefines the obsolete STR2CSTR
#define STR2CSTR(x) rb_str2cstr((VALUE)(x),0)


void Init_dhkeyx() {
  rb_cDHKey = rb_define_class("DHKey", rb_cObject);
  rb_define_singleton_method(rb_cDHKey, "new", rb_DH_key_new, -2);
	rb_define_method(rb_cDHKey, "initialize", rb_DH_key_initialize, -2);

#define def_get_attr(n)	rb_define_method(rb_cDHKey, #n, rb_DH_key_get_ ## n, 0);
	def_get_attr(base)
	def_get_attr(prime)
	def_get_attr(private_key)
	def_get_attr(my_public_key)
	def_get_attr(their_public_key)
	def_get_attr(shared_key)
	
	rb_define_method(rb_cDHKey, "base=" , rb_DH_key_set_base, 1);
	rb_define_method(rb_cDHKey, "prime=" , rb_DH_key_set_prime, 1);
	rb_define_method(rb_cDHKey, "private_key=" , rb_DH_key_set_private_key, 1);
	rb_define_method(rb_cDHKey, "their_public_key=" , rb_DH_key_set_their_public_key, 1);
	
	rb_define_alias(rb_cDHKey, "g", "base");
	rb_define_alias(rb_cDHKey, "p", "prime");
	rb_define_alias(rb_cDHKey, "a", "private_key");
	rb_define_alias(rb_cDHKey, "x", "my_public_key");
	rb_define_alias(rb_cDHKey, "y", "their_public_key");
	rb_define_alias(rb_cDHKey, "k", "shared_key");
	
	rb_define_alias(rb_cDHKey, "g=", "base=");
	rb_define_alias(rb_cDHKey, "p=", "prime=");
	rb_define_alias(rb_cDHKey, "a=", "private_key=");
	rb_define_alias(rb_cDHKey, "y=", "their_public_key=");
}

static VALUE rb_DH_key_new(VALUE self, VALUE args) {
	VALUE argv[3];
  struct dh_DHKey * data = ALLOC(struct dh_DHKey);
  VALUE obj = Data_Wrap_Struct(self, 0, dh_DHKey_free, data);
	
	data->ctx = BN_CTX_new();
	data->base = BN_new();
	data->prime = BN_new();
	data->private_key = BN_new();
	data->my_public_key = BN_new();
	data->their_public_key = BN_new();
	data->shared_key = BN_new();
	
	BN_zero(data->base);
	BN_zero(data->prime);
	BN_zero(data->private_key);
	BN_zero(data->my_public_key);
	BN_zero(data->their_public_key);
	BN_zero(data->shared_key);
	
	if (RARRAY_LEN(args) < 1)	rb_DH_key_set_base(obj, Qnil);
	else												rb_DH_key_set_base(obj, rb_ary_entry(args, 0));
	if (RARRAY_LEN(args)  < 2)	rb_DH_key_set_prime(obj, Qnil);
	else												rb_DH_key_set_prime(obj, rb_ary_entry(args,1));
	if (RARRAY_LEN(args)  < 3)	rb_DH_key_set_private_key(obj, Qnil);
	else												rb_DH_key_set_private_key(obj, rb_ary_entry(args, 2));
	
	argv[0] = (VALUE)rb_DH_key_get_base;
  argv[1] = (VALUE)rb_DH_key_get_prime;
  argv[2] = (VALUE)rb_DH_key_get_private_key;
  rb_obj_call_init(obj, 3, argv);
	
  return obj;
}

static VALUE rb_DH_key_initialize(VALUE self, VALUE args) {
	return self;
}


static VALUE rb_DH_key_get_base (VALUE self) {
  struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	return rb_str_new2(BN_bn2dec(ptr->base));
}
static VALUE rb_DH_key_get_prime (VALUE self) {
  struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	return rb_str_new2(BN_bn2dec(ptr->prime));
}
static VALUE rb_DH_key_get_private_key (VALUE self) {
  struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	return rb_str_new2(BN_bn2dec(ptr->private_key));
}
static VALUE rb_DH_key_get_my_public_key (VALUE self) {
  struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	return rb_str_new2(BN_bn2dec(ptr->my_public_key));
}
static VALUE rb_DH_key_get_their_public_key (VALUE self) {
  struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	return rb_str_new2(BN_bn2dec(ptr->their_public_key));
}
static VALUE rb_DH_key_get_shared_key (VALUE self) {
  struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	return rb_str_new2(BN_bn2dec(ptr->shared_key));
}

#define TO_STR(x) #x
#define DH_RAND_RANGE TO_STR(9999999999999999999999999999999999999)
static BIGNUM * rb_DH_rand_range = NULL;

static VALUE rb_DH_key_set_base(VALUE self, VALUE v) {
	struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	if (NIL_P(v) == 0) {
		if (rb_respond_to(v, rb_intern("to_s")) != 0) {
			BN_dec2bn(&(ptr->base), STR2CSTR(rb_funcall(v, rb_intern("to_s"), 0)));
		} else {
			rb_raise(rb_eRuntimeError, "%s must respond to 'to_s'", "base");
			return Qnil;
		}
	} else {
		if (rb_DH_rand_range == NULL) BN_dec2bn(&rb_DH_rand_range, DH_RAND_RANGE);
		BN_rand_range(ptr->base, rb_DH_rand_range);
	}
	
	if ((BN_is_zero(ptr->base) == 0) && (BN_is_zero(ptr->prime) == 0) && (BN_is_zero(ptr->private_key) == 0)) {
		if (BN_mod_exp(ptr->my_public_key, ptr->base, ptr->private_key, ptr->prime, ptr->ctx) == 0) {
			rb_raise(rb_eRuntimeError, "failed to calculate");
			return Qnil;
		}
	}
	
	return rb_DH_key_get_base(self);
}
	
static VALUE rb_DH_key_set_prime(VALUE self, VALUE v) {
	struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	if (NIL_P(v) == 0) {
		if (rb_respond_to(v, rb_intern("to_s")) != 0) {
			BN_dec2bn(&(ptr->prime), STR2CSTR(rb_funcall(v, rb_intern("to_s"), 0)));
		} else {
			rb_raise(rb_eRuntimeError, "%s must respond to 'to_s'", "prime");
			return Qnil;
		}
	} else {
		BN_dec2bn(&(ptr->prime), "155172898181473697471232257763715539915724801966915404479707795314057629378541917580651227423698188993727816152646631438561595825688188889951272158842675419950341258706556549803580104870537681476726513255747040765857479291291572334510643245094715007229621094194349783925984760375594985848253359305585439638443");
	}
	
	if ((BN_is_zero(ptr->base) == 0) && (BN_is_zero(ptr->prime) == 0) && (BN_is_zero(ptr->private_key) == 0)) {
		if (BN_mod_exp(ptr->my_public_key, ptr->base, ptr->private_key, ptr->prime, ptr->ctx) == 0) {
			rb_raise(rb_eRuntimeError, "failed to calculate");
			return Qnil;
		}
	}
	
	return rb_DH_key_get_prime(self);
}
	
static VALUE rb_DH_key_set_private_key(VALUE self, VALUE v) {
	struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	if (NIL_P(v) == 0) {
		if (rb_respond_to(v, rb_intern("to_s")) != 0) {
			BN_dec2bn(&(ptr->private_key), STR2CSTR(rb_funcall(v, rb_intern("to_s"), 0)));
		} else {
			rb_raise(rb_eRuntimeError, "%s must respond to 'to_s'", "private_key");
			return Qnil;
		}
	} else {
		if (rb_DH_rand_range == NULL) BN_dec2bn(&rb_DH_rand_range, DH_RAND_RANGE);
		BN_rand_range(ptr->private_key, rb_DH_rand_range);
	}
	
	if ((BN_is_zero(ptr->base) == 0) && (BN_is_zero(ptr->prime) == 0) && (BN_is_zero(ptr->private_key) == 0)) {
		if (BN_mod_exp(ptr->my_public_key, ptr->base, ptr->private_key, ptr->prime, ptr->ctx) == 0) {
			rb_raise(rb_eRuntimeError, "failed to calculate");
			return Qnil;
		}
	}
	
	return rb_DH_key_get_private_key(self);
}
	
static VALUE rb_DH_key_set_their_public_key(VALUE self, VALUE v) {
	struct dh_DHKey * ptr; Data_Get_Struct(self, struct dh_DHKey, ptr);
	if (NIL_P(v) == 0) {
		if (rb_respond_to(v, rb_intern("to_s")) != 0) {
			BN_dec2bn(&(ptr->their_public_key), STR2CSTR(rb_funcall(v, rb_intern("to_s"), 0)));
		} else {
			rb_raise(rb_eRuntimeError, "%s must respond to 'to_s'", "their_public_key");
			return Qnil;
		}
	} else {
		rb_raise(rb_eRuntimeError, "%s must not be nil", "their_public_key");
		return Qnil;
	}
	
	if ((BN_is_zero(ptr->their_public_key) == 0) && (BN_is_zero(ptr->prime) == 0) && (BN_is_zero(ptr->private_key) == 0)) {
		if (BN_mod_exp(ptr->shared_key, ptr->their_public_key, ptr->private_key, ptr->prime, ptr->ctx) == 0) {
			rb_raise(rb_eRuntimeError, "failed to calculate");
			return Qnil;
		}
	}
	
	return rb_DH_key_get_their_public_key(self);
}

void dh_DHKey_free(struct dh_DHKey* key) {
	if (key != NULL) {
		if (key->base != NULL) BN_clear_free(key->base);
		if (key->prime != NULL) BN_clear_free(key->prime);
		if (key->private_key != NULL) BN_clear_free(key->private_key);
		if (key->my_public_key != NULL) BN_clear_free(key->my_public_key);
		if (key->their_public_key != NULL) BN_clear_free(key->their_public_key);
		if (key->shared_key != NULL) BN_clear_free(key->shared_key);
		if (key->ctx != NULL) BN_CTX_free(key->ctx);
		free(key);
	}
}