#include <jni.h>
#include <jni_md.h>
#include <stdio.h>
#include <stdlib.h>
#include "Encrypt.h"

void encrypt (long *v, long *k){
/* TEA encryption algorithm */
unsigned long y = v[0], z=v[1], sum = 0;
unsigned long delta = 0x9e3779b9, n=32;

	while (n-- > 0){
		sum += delta;
		y += (z<<4) + k[0] ^ z + sum ^ (z>>5) + k[1];
		z += (y<<4) + k[2] ^ y + sum ^ (y>>5) + k[3];
	}

	v[0] = y;
	v[1] = z;
}



void decrypt (long *v, long *k){
/* TEA decryption routine */
unsigned long n=32, sum, y=v[0], z=v[1];
unsigned long delta=0x9e3779b9l;

	sum = delta<<5;
	while (n-- > 0){
		z -= (y<<4) + k[2] ^ y + sum ^ (y>>5) + k[3];
		y -= (z<<4) + k[0] ^ z + sum ^ (z>>5) + k[1];
		sum -= delta;
	}
	v[0] = y;
	v[1] = z;
}

JNIEXPORT void JNICALL Java_Encrypt_encrypt
  (JNIEnv *env, jobject thisObj, jbyteArray value, jbyteArray key){

  	printf("encrypt\n");
  	jbyte *v = (*env)->GetByteArrayElements(env, value, 0);
  	jbyte *k = (*env)->GetByteArrayElements(env, key, 0);
    jboolean is_copy_value;
    jboolean is_copy_key;
  	jsize vSize = (*env)->GetArrayLength(env, value);

    long *a = (long *) v;
    long *b = (long *) k;

  	int i = 0;
  	for(i = 0; i<vSize; i+=2){
  		encrypt(a, b);
  	}

    (*env)->ReleaseByteArrayElements(env, value, v, is_copy_value);
    (*env)->ReleaseByteArrayElements(env, key, k, is_copy_key);

  }


JNIEXPORT void JNICALL Java_Encrypt_decrypt
  (JNIEnv *env, jobject thisObj, jbyteArray value, jbyteArray key){

    jboolean is_copy_key;
    jboolean is_copy_value;
  	jbyte *v = (*env)->GetByteArrayElements(env, value, 0);
  	jbyte *k = (*env)->GetByteArrayElements(env, key, 0);

    long *a = (long *) v;
    long *b = (long *) k;

  	jsize vSize = (*env)->GetArrayLength(env, value);
  	int i = 0;
  	for(i = 0; i<vSize; i+=2){
  		decrypt(v, k)
  	}
    (*env)->ReleaseByteArrayElements(env, value, v, is_copy_value);
    (*env)->ReleaseByteArrayElements(env, key, k, is_copy_key);
  	
    printf("decrypt\n");
  }



