/* Version: mss_v6_3 */
/*
 * secmod.h
 *
 * General Public Crypto Definitions & Types Header
 *
 * Copyright Mocana Corp 2006-2007. All Rights Reserved.
 * Proprietary and Confidential Material.
 *
 */


/*------------------------------------------------------------------*/

#ifdef __ENABLE_MOCANA_HW_SECURITY_MODULE__
enum secModTypes
{
    secmod_undefined = 0,
#ifdef __ENABLE_MOCANA_TPM__
    secmod_TPM = 1,
#endif
};

struct AsymmetricKey;

typedef struct secModDescr
{
    enum secModTypes    type;
    ubyte4              id;
} secModDescr;

/*if a secmod is enabled, this will allow the context to be passed into the CRYPTO_* functions in pubcrypto.c*/
#define MOC_SECMOD(X)   X,
#else
#define MOC_SECMOD(X)
#endif

#ifdef __ENABLE_MOCANA_HW_SECURITY_MODULE__
/* Initialize the hardware security module */
MSTATUS SECMOD_init(secModDescr* secModContext, ubyte4 secmodType, void* params);

/* Create AsymmetricKey using a hardware security module */
MSTATUS SECMOD_createKey(secModDescr* secModContext, ubyte4 keyType, void* params, struct AsymmetricKey* pKey);

/* Validate whether the key is a properly formed key used by the hardware security module */
MSTATUS SECMOD_validateKey(secModDescr* secModContext, struct AsymmetricKey* pKey, byteBoolean* isValid);

/* Serialize in a key generated by a hardware security module */
MSTATUS SECMOD_serializeIn(ubyte* pHWKeyBlob, ubyte4 hwKeyBlobLen, struct AsymmetricKey* pKey);

/* Serialize out a key generated by a hardware security module */
MSTATUS SECMOD_serializeOut(struct AsymmetricKey* pKey, ubyte** pHWKeyBlob, ubyte4* pHWKeyBlobLen);

/* Uninitialize the hardware security module */
MSTATUS SECMOD_uninit(secModDescr* secModContext);

/* Retrieve meta info about the security module */
MSTATUS SECMOD_info(secModDescr* secModContext);

/* Take ownership of the security module */
MSTATUS SECMOD_takeOwnership(secModDescr* secModContext, void* params);

/* Clear the hardware security module */
MSTATUS SECMOD_clear(secModDescr* secModContext);
#endif