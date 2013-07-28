/******----------------------- File Name: Alert_tests_suite.h ---------------------------******

Created by: Senka Kojic on 09/12/2012.
Tested by: Full Name on DD/MM/YYYY.
Modified by: Fadwa & Senka
Version: 1.0
Copyright (c) 2012, MIBitcoinc


Brief Description:
------------------
This file includes basic test cases related to Alerts. It includes functions:test_case_Deserialization, test_case_Serialization.
More functionalities need to be examined.


******--------------------------------------------------------------------******/

#include <stdio.h>
#include <stdlib.h>
#include "stdarg.h"
#include <time.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
/*#include "CUnit/TBTest.h"*/
/*#include "CUnit/Automated.h"
 #include "CUnit/Console.h"*/
#include "../src/Constants.h"
#include "openssl/ssl.h"
#include "openssl/ripemd.h"
#include "openssl/rand.h"
#include "../src/Object/Message/Alert.h"




void err6(Error a, char * format, ...);
void err6(Error a, char * format, ...) {
	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
	printf("\n");
}

/* Test Suite setup and cleanup functions: */

int init_Alert_suite(void) {
	return 0;
}

int clean_Alert_suite(void) {
	return 0;
}

/************* Test case functions ****************/

/* Test genesis block */

void test_case_Deserialization(void){
	uint8_t data[188] = {
	0x73, /* Length of payload*/
	0x01,0x00,0x00,0x00, /* Version 1*/
	0x37,0x66,0x40,0x4F,0x00,0x00,0x00,0x00, /* Relay until 1329620535*/
	0xB3,0x05,0x43,0x4F,0x00,0x00,0x00,0x00, /* Expires at 1329792435*/
	0xF2,0x03,0x00,0x00, /* ID 1010*/
	0xF1,0x03,0x00,0x00, /* Cancel < 1009*/
	0x00, /* No more IDs*/
	0x10,0x27,0x00,0x00, /* Min version 10000*/
	0x48,0xee,0x00,0x00, /* Max version 61000*/
	0x00, /* No user agents*/
	0x64,0x00,0x00,0x00, /* Priority 100*/
	0x00, /* Empty hidden comment*/
	0x46, /* Displayed comment is 70 characters long*/
	/* "See bitcoin.org/feb20 if you have trouble connecting after 20 February"*/
	0x53,0x65,0x65,0x20,0x62,0x69,0x74,0x63,0x6F,0x69,0x6E,0x2E,0x6F,0x72,0x67,0x2F,0x66,0x65,0x62,0x32,0x30,0x20,0x69,0x66,0x20,0x79,0x6F,0x75,0x20,0x68,0x61,0x76,0x65,0x20,0x74,0x72,0x6F,0x75,0x62,0x6C,0x65,0x20,0x63,0x6F,0x6E,0x6E,0x65,0x63,0x74,0x69,0x6E,0x67,0x20,0x61,0x66,0x74,0x65,0x72,0x20,0x32,0x30,0x20,0x46,0x65,0x62,0x72,0x75,0x61,0x72,0x79,
	0x00, /* No reserved*/
	/* Signature*/
	0x47, /* Signature is 71 bytes long*/
	0x30,0x45,0x02,0x21,0x00,0x83,0x89,0xdf,0x45,0xF0,0x70,0x3F,0x39,0xEC,0x8C,0x1C,0xC4,0x2C,0x13,0x81,0x0F,0xFC,0xAE,0x14,0x99,0x5B,0xB6,0x48,0x34,0x02,0x19,0xE3,0x53,0xB6,0x3B,0x53,0xEB,0x02,0x20,0x09,0xEC,0x65,0xE1,0xC1,0xAA,0xEE,0xC1,0xFD,0x33,0x4C,0x6B,0x68,0x4B,0xDE,0x2B,0x3F,0x57,0x30,0x60,0xD5,0xb7,0x0C,0x3A,0x46,0x72,0x33,0x26,0xE4,0xE8,0xA4,0xF1
	};
	/*Basic testing, additional functions haven't been tested*/
	ByteArray * bytes = createNewByteArrayUsingDataCopy(data, 188, err6);
	Alert * alert = newAlertFromData(bytes, err6);
	CU_ASSERT_EQUAL(deserializeAlert(alert),188);
	CU_ASSERT_EQUAL(alert->version,1);
	CU_ASSERT_EQUAL(alert->relayUntil,1329620535);
	CU_ASSERT_EQUAL(alert->expiration, 1329792435);
	CU_ASSERT_EQUAL(alert->ID,1010);
	CU_ASSERT_EQUAL(alert->cancel,1009);
	CU_ASSERT_EQUAL(alert->setCancelNum, 0);
	CU_ASSERT_EQUAL(alert->setCancel, NULL);
	CU_ASSERT_EQUAL(alert->minVer, 10000);
	CU_ASSERT_EQUAL(alert->maxVer, 61000);
	CU_ASSERT_EQUAL(alert->userAgentNum, 0);
	CU_ASSERT_EQUAL(alert->userAgents, NULL);
	CU_ASSERT_EQUAL(alert->priority, 100);
	CU_ASSERT_EQUAL(alert->hiddenComment, NULL);
	CU_ASSERT_EQUAL(memcmp(getByteArrayData(alert->displayedComment),"See bitcoin.org/feb20 if you have trouble connecting after 20 February",70),0);
	CU_ASSERT_EQUAL(alert->reserved, NULL);

	ByteArray * payload = alertGetPayload(alert);
	uint8_t hash1[32];
	Sha256(getByteArrayData(payload),payload->length,hash1);
	uint8_t hash2[32];
	Sha256(hash1, 32,hash2);
	CU_ASSERT (ECDSAVerify(getByteArrayData(alert->signature),alert->signature->length,hash2,(uint8_t [65]){0x04,0xFC,0x97,0x02,0x84,0x78,0x40,0xAA,0xF1,0x95,0xDE,0x84,0x42,0xEB,0xEC,0xED,0xF5,0xB0,0x95,0xCD,0xBB,0x9B,0xC7,0x16,0xBD,0xA9,0x11,0x09,0x71,0xB2,0x8A,0x49,0xE0,0xEA,0xD8,0x56,0x4F,0xF0,0xDB,0x22,0x20,0x9E,0x03,0x74,0x78,0x2C,0x09,0x3B,0xB8,0x99,0x69,0x2D,0x52,0x4E,0x9D,0x6A,0x69,0x56,0xE7,0xC5,0xEC,0xBC,0xD6,0x82,0x84},65));

}


void test_case_Serialization(void){
	uint8_t data[188] = {
		0x73, /* Length of payload*/
		0x01,0x00,0x00,0x00, /* Version 1*/
		0x37,0x66,0x40,0x4F,0x00,0x00,0x00,0x00, /* Relay until 1329620535*/
		0xB3,0x05,0x43,0x4F,0x00,0x00,0x00,0x00, /* Expires at 1329792435*/
		0xF2,0x03,0x00,0x00, /* ID 1010*/
		0xF1,0x03,0x00,0x00, /* Cancel < 1009*/
		0x00, /* No more IDs*/
		0x10,0x27,0x00,0x00, /* Min version 10000*/
		0x48,0xee,0x00,0x00, /* Max version 61000*/
		0x00, /* No user agents*/
		0x64,0x00,0x00,0x00, /* Priority 100*/
		0x00, /* Empty hidden comment*/
		0x46, /* Displayed comment is 70 characters long*/
		/* "See bitcoin.org/feb20 if you have trouble connecting after 20 February"*/
		0x53,0x65,0x65,0x20,0x62,0x69,0x74,0x63,0x6F,0x69,0x6E,0x2E,0x6F,0x72,0x67,0x2F,0x66,0x65,0x62,0x32,0x30,0x20,0x69,0x66,0x20,0x79,0x6F,0x75,0x20,0x68,0x61,0x76,0x65,0x20,0x74,0x72,0x6F,0x75,0x62,0x6C,0x65,0x20,0x63,0x6F,0x6E,0x6E,0x65,0x63,0x74,0x69,0x6E,0x67,0x20,0x61,0x66,0x74,0x65,0x72,0x20,0x32,0x30,0x20,0x46,0x65,0x62,0x72,0x75,0x61,0x72,0x79,
		0x00, /* No reserved*/
		/* Signature*/
		0x47, /* Signature is 71 bytes long*/
		0x30,0x45,0x02,0x21,0x00,0x83,0x89,0xdf,0x45,0xF0,0x70,0x3F,0x39,0xEC,0x8C,0x1C,0xC4,0x2C,0x13,0x81,0x0F,0xFC,0xAE,0x14,0x99,0x5B,0xB6,0x48,0x34,0x02,0x19,0xE3,0x53,0xB6,0x3B,0x53,0xEB,0x02,0x20,0x09,0xEC,0x65,0xE1,0xC1,0xAA,0xEE,0xC1,0xFD,0x33,0x4C,0x6B,0x68,0x4B,0xDE,0x2B,0x3F,0x57,0x30,0x60,0xD5,0xb7,0x0C,0x3A,0x46,0x72,0x33,0x26,0xE4,0xE8,0xA4,0xF1
		};
/*Basic testing, additional functions haven't been tested*/
ByteArray * bytes = createNewByteArrayUsingDataCopy(data, 188, err6);
Alert * alert = newAlertFromData(bytes, err6);
deserializeAlert(alert); /*in all cases of serialization I need to deserialize first, otherwise I get segmentation error*/
ByteArray * payload = alertGetPayload(alert);
uint8_t hash1[32];
Sha256(getByteArrayData(payload),payload->length,hash1);
uint8_t hash2[32];
Sha256(hash1, 32,hash2);
alertSerialisePayload(alert);
CU_ASSERT_EQUAL(alertSerialiseSignature(alert, 116),188);
CU_ASSERT(!memcmp(data, getByteArrayData(bytes), 188));
}



/* Grouping all test cases into an array that will be registered later with a single test suite. */
CU_TestInfo alert_test_array[] = {
  { "test_case_Deserialization", test_case_Deserialization },
  { "test_case_Serialization", test_case_Serialization},
  CU_TEST_INFO_NULL,
};
