#include <QRandomGenerator64>
#include <aes.h>
#include <gcm.h>
#include <filters.h>
#include <secblock.h>
#include <randpool.h>
#include <iostream>
using namespace CryptoPP;
using namespace std;

void testCryptopp()
{
	AutoSeededRandomPool prng;

	SecByteBlock key(AES::DEFAULT_KEYLENGTH);
	prng.GenerateBlock( key, key.size() );

	SecByteBlock iv(AES::BLOCKSIZE);
	prng.GenerateBlock( iv, iv.size() );

	// Plain text
	string pdata="Authenticated Encryption";

	// Encrypted, with Tag
	string cipher, encoded;

	// Recovered plain text
	string rpdata;

	/*********************************\
	\*********************************/

	try
	{
		GCM< AES >::Encryption e;
		e.SetKeyWithIV( key, key.size(), iv, sizeof(iv) );

		StringSource ss1( pdata, true,
			new AuthenticatedEncryptionFilter( e,
				new StringSink( cipher ), false
			) // AuthenticatedEncryptionFilter
		); // StringSource
	}
	catch( CryptoPP::Exception& e )
	{
		cerr << e.what() << endl;
		exit(1);
	}

	/*********************************\
	\*********************************/

	try
	{
		GCM< AES >::Decryption d;
		d.SetKeyWithIV( key, key.size(), iv, sizeof(iv) );

		AuthenticatedDecryptionFilter df( d,
			new StringSink( rpdata )
		); // AuthenticatedDecryptionFilter

		// The StringSource dtor will be called immediately
		//  after construction below. This will cause the
		//  destruction of objects it owns. To stop the
		//  behavior so we can get the decoding result from
		//  the DecryptionFilter, we must use a redirector
		//  or manually Put(...) into the filter without
		//  using a StringSource.
		StringSource ss2( cipher, true,
			new Redirector( df /*, PASS_EVERYTHING */ )
		); // StringSource

		// If the object does not throw, here's the only
		//  opportunity to check the data's integrity
		if( true == df.GetLastResult() ) {
			cout << "recovered text: " << rpdata << endl;
		}
	}
	catch( CryptoPP::Exception& e )
	{
		cerr << e.what() << endl;
		exit(1);
	}
}
