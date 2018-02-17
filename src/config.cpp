#include "config.h"
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_partition.h>

struct nvs_entry
{
	uint8_t Ns;	// Namespace ID
	uint8_t Type;  // Type of value
	uint8_t Span;  // Number of entries used for this item
	uint8_t Rvs;   // Reserved, should be 0xFF
	uint32_t CRC;  // CRC
	char Key[16];  // Key in Ascii
	uint64_t Data; // Data in entry
};

struct nvs_page // For nvs entries
{				// 1 page is 4096 bytes
	uint32_t State;
	uint32_t Seqnr;

	uint32_t Unused[5];
	uint32_t CRC;
	uint8_t Bitmap[32];
	nvs_entry Entry[126];
};

// Common data
nvs_page buf;
nvs_handle config_nvs = 0;
void dumpNVS();

//--------------------------------------------------------------------
String GetNvsString(nvs_handle handle, const char *name, const char *defaultVal)
{
	if (handle == 0)
		return defaultVal;

	size_t required_size;
	if (nvs_get_str(handle, name, NULL, &required_size) != ESP_OK)
		return defaultVal;

	char *buf = (char *)malloc(required_size);

	nvs_get_str(handle, name, buf, &required_size);

	String res(buf);
	free(buf);

	return res;
}
//--------------------------------------------------------------------
String GetCfgString(const char *key, const char *defaultVal)
{
	return (GetNvsString(config_nvs, key, defaultVal));
}

//--------------------------------------------------------------------
int GetCfgInt(const char *key, int defaultVal)
{
	if (config_nvs == 0)
		return defaultVal;

	int res;
	if (nvs_get_i32(config_nvs, key, &res) != ESP_OK)
		return defaultVal;
	return res;
}

//--------------------------------------------------------------------
void SetCfgString(const char *key, const String &val)
{
	// check that the value has changed
	if (GetCfgString(key, "") != val)
		nvs_set_str(config_nvs, key, val.c_str());
}
//--------------------------------------------------------------------
void SetCfgInt(const char *key, int val)
{
	// check that the value has changed
	if (GetCfgInt(key, 0) != val)
		nvs_set_i32(config_nvs, key, val);
}

//--------------------------------------------------------------------
void init_config()
{
	if (nvs_open("config", NVS_READWRITE, &config_nvs) != ESP_OK)
	{
		config_nvs = 0;
		Serial.println("Failed to open the CONFIG nvs.");
		return;
	}

	dumpNVS();
}

// Find the namespace ID for the namespace passed as parameter.
//--------------------------------------------------------------------
uint8_t FindNsID(const esp_partition_t *nvs, const char *ns)
{
	uint32_t offset = 0; // Offset in nvs partition
	uint8_t res = 0xFF;  // Function result

	while (offset < nvs->size)
	{
		// Read 1 page in nvs partition
		if (esp_partition_read(nvs, offset, &buf, sizeof(nvs_page)) != ESP_OK)
		{
			Serial.printf("Error reading NVS!\n");
			break;
		}

		uint8_t i = 0; // Index in Entry 0..125
		while (i < 126)
		{
			uint8_t bm = (buf.Bitmap[i / 4] >> ((i % 4) * 2)) & 0x03; // Get bitmap for this entry

			if ((bm == 2) && (buf.Entry[i].Ns == 0) && (strcmp(ns, buf.Entry[i].Key) == 0))
			{
				res = buf.Entry[i].Data & 0xFF; // Return the ID
				offset = nvs->size;				// Stop outer loop as well
				break;
			}
			else
			{
				if (bm == 2)
					i += buf.Entry[i].Span; // Next entry
				else
					i++;
			}
		}
		offset += sizeof(nvs_page); // Prepare to read next page in nvs
	}
	return res;
}

//--------------------------------------------------------------------
void dumpNVS()
{
	// Get NVS partition iterator
	esp_partition_iterator_t pi = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "nvs");
	if (pi == NULL)
		return;

	const esp_partition_t *nvs = esp_partition_get(pi);		// Get partition struct
	esp_partition_iterator_release(pi); // Release the iterator

	uint8_t namespace_ID = FindNsID(nvs, "config"); // Find ID of our namespace in NVS
	Serial.printf("Namespace ID is %d\n", namespace_ID);

	uint32_t offset = 0;  // Offset in nvs partition
	while (offset < nvs->size)
	{
		 // Read 1 page in nvs partition
		if (esp_partition_read(nvs, offset, &buf, sizeof(nvs_page)) != ESP_OK)
		{
			Serial.printf("Error reading NVS!\n");
			return;
		}

		uint8_t i = 0;
		while (i < 126)
		{
			uint8_t bm = (buf.Bitmap[i / 4] >> ((i % 4) * 2)) & 0x03; // Get bitmap for this entry
			if (bm == 2)
			{
				if ((namespace_ID == 0xFF) || (buf.Entry[i].Ns == namespace_ID))
					Serial.printf("Key %03d: %s\n", i, buf.Entry[i].Key); // Print the key

				i += buf.Entry[i].Span; // Next entry
			}
			else
				i++;
		}
		offset += sizeof(nvs_page); // Prepare to read next page in nvs
	}
}