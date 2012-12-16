/* ecrypt-sync.h */

/* 
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 * 
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef GRAIN_SYNC
#define GRAIN_SYNC

#include "../ecrypt-portable.h"
#include "../../estreamInterface.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/* 
 * The name of your cipher.
 */
#define GRAIN_NAME "Grain-v1"    /* [edit] */ 
#define GRAIN_PROFILE "___H3"
/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; GRAIN_KEYSIZE(i) <= GRAIN_MAXKEYSIZE; ++i)
 *   {
 *     keysize = GRAIN_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define GRAIN_MAXKEYSIZE 128                 /* [edit] */
#define GRAIN_KEYSIZE(i) (128 + (i))      /* [edit] */

#define GRAIN_MAXIVSIZE 96                   /* [edit] */
#define GRAIN_IVSIZE(i) (96 + (i))        /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/* 
 * GRAIN_ctx is the structure containing the representation of the
 * internal state of your cipher. 
 */

typedef struct
{
	u32 LFSR[128];
	u32 NFSR[128];
	const u8* p_key;
	u32 keysize;
	u32 ivsize;

} GRAIN_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_Grain: public EstreamInterface
{
public:
	/* Mandatory functions */

	/*
	 * Key and message independent initialization. This function will be
	 * called once when the program starts (e.g., to build expanded S-box
	 * tables).
	 */
	void ECRYPT_init();

	/*
	 * Key setup. It is the user's responsibility to select the values of
	 * keysize and ivsize from the set of supported values specified
	 * above.
	 */
	void ECRYPT_keysetup(
	  void* ctx, 
	  const u8* key, 
	  u32 keysize,                /* Key size in bits. */ 
	  u32 ivsize);                /* IV size in bits. */ 

	/*
	 * IV setup. After having called ECRYPT_keysetup(), the user is
	 * allowed to call ECRYPT_ivsetup() different times in order to
	 * encrypt/decrypt different messages with the same key but different
	 * IV's.
	 */
	void ECRYPT_ivsetup(
	  void* ctx, 
	  const u8* iv);

	/*
	 * Encryption/decryption of arbitrary length messages.
	 *
	 * For efficiency reasons, the API provides two types of
	 * encrypt/decrypt functions. The ECRYPT_encrypt_bytes() function
	 * (declared here) encrypts byte strings of arbitrary length, while
	 * the ECRYPT_encrypt_blocks() function (defined later) only accepts
	 * lengths which are multiples of ECRYPT_BLOCKLENGTH.
	 * 
	 * The user is allowed to make multiple calls to
	 * ECRYPT_encrypt_blocks() to incrementally encrypt a long message,
	 * but he is NOT allowed to make additional encryption calls once he
	 * has called ECRYPT_encrypt_bytes() (unless he starts a new message
	 * of course). For example, this sequence of calls is acceptable:
	 *
	 * ECRYPT_keysetup();
	 *
	 * ECRYPT_ivsetup();
	 * ECRYPT_encrypt_blocks();
	 * ECRYPT_encrypt_blocks();
	 * ECRYPT_encrypt_bytes();
	 *
	 * ECRYPT_ivsetup();
	 * ECRYPT_encrypt_blocks();
	 * ECRYPT_encrypt_blocks();
	 *
	 * ECRYPT_ivsetup();
	 * ECRYPT_encrypt_bytes();
	 * 
	 * The following sequence is not:
	 *
	 * ECRYPT_keysetup();
	 * ECRYPT_ivsetup();
	 * ECRYPT_encrypt_blocks();
	 * ECRYPT_encrypt_bytes();
	 * ECRYPT_encrypt_blocks();
	 */

	void ECRYPT_encrypt_bytes(
	  void* ctx, 
	  const u8* plaintext, 
	  u8* ciphertext, 
	  u32 msglen);                /* Message length in bytes. */ 

	void ECRYPT_decrypt_bytes(
	  void* ctx, 
	  const u8* ciphertext, 
	  u8* plaintext, 
	  u32 msglen);                /* Message length in bytes. */ 

	/* ------------------------------------------------------------------------- */

	/* Optional features */

	/* 
	 * For testing purposes it can sometimes be useful to have a function
	 * which immediately generates keystream without having to provide it
	 * with a zero plaintext. If your cipher cannot provide this function
	 * (e.g., because it is not strictly a synchronous cipher), please
	 * reset the GRAIN_GENERATES_KEYSTREAM flag.
	 */

	#define GRAIN_GENERATES_KEYSTREAM
	#ifdef GRAIN_GENERATES_KEYSTREAM

	void GRAIN_keystream_bytes(
	  GRAIN_ctx* ctx,
	  u8* keystream,
	  u32 length);                /* Length of keystream in bytes. */

	#endif

	/* ------------------------------------------------------------------------- */

	/* Optional optimizations */

	/* 
	 * By default, the functions in this section are implemented using
	 * calls to functions declared above. However, you might want to
	 * implement them differently for performance reasons.
	 */

	/*
	 * All-in-one encryption/decryption of (short) packets.
	 *
	 * The default definitions of these functions can be found in
	 * "ecrypt-sync.c". If you want to implement them differently, please
	 * undef the GRAIN_USES_DEFAULT_ALL_IN_ONE flag.
	 */
	#define GRAIN_USES_DEFAULT_ALL_IN_ONE        /* [edit] */

	void GRAIN_encrypt_packet(
	  GRAIN_ctx* ctx, 
	  const u8* iv,
	  const u8* plaintext, 
	  u8* ciphertext, 
	  u32 msglen);

	void GRAIN_decrypt_packet(
	  GRAIN_ctx* ctx, 
	  const u8* iv,
	  const u8* ciphertext, 
	  u8* plaintext, 
	  u32 msglen);

	/*
	 * Encryption/decryption of blocks.
	 * 
	 * By default, these functions are defined as macros. If you want to
	 * provide a different implementation, please undef the
	 * GRAIN_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
	 * declared below.
	 */

	#define GRAIN_BLOCKLENGTH 4                  /* [edit] */

	#define GRAIN_USES_DEFAULT_BLOCK_MACROS      /* [edit] */
	#ifdef GRAIN_USES_DEFAULT_BLOCK_MACROS

	#define GRAIN_encrypt_blocks(ctx, plaintext, ciphertext, blocks)  \
	  ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext,                 \
		(blocks) * GRAIN_BLOCKLENGTH)

	#define GRAIN_decrypt_blocks(ctx, ciphertext, plaintext, blocks)  \
	  ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext,                 \
		(blocks) * GRAIN_BLOCKLENGTH)

	#ifdef GRAIN_GENERATES_KEYSTREAM

	#define GRAIN_keystream_blocks(ctx, keystream, blocks)            \
	  GRAIN_AE_keystream_bytes(ctx, keystream,                        \
		(blocks) * GRAIN_BLOCKLENGTH)

	#endif

	#else

	void GRAIN_encrypt_blocks(
	  GRAIN_ctx* ctx, 
	  const u8* plaintext, 
	  u8* ciphertext, 
	  u32 blocks);                /* Message length in blocks. */ 

	void GRAIN_decrypt_blocks(
	  GRAIN_ctx* ctx, 
	  const u8* ciphertext, 
	  u8* plaintext, 
	  u32 blocks);                /* Message length in blocks. */ 

	#ifdef GRAIN_GENERATES_KEYSTREAM

	void GRAIN_keystream_blocks(
	  GRAIN_AE_ctx* ctx,
	  const u8* keystream,
	  u32 blocks);                /* Keystream length in blocks. */ 

	#endif

	#endif
};
/*
 * If your cipher can be implemented in different ways, you can use
 * the GRAIN_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DGRAIN_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (GRAIN_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same GRAIN_BLOCKLENGTH, etc.). 
 */
#define GRAIN_MAXVARIANT 1                   /* [edit] */

#ifndef GRAIN_VARIANT
#define GRAIN_VARIANT 1
#endif

#if (GRAIN_VARIANT > GRAIN_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
