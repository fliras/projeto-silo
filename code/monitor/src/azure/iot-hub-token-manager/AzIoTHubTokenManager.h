// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef AZIOTSASTOKEN_H
#define AZIOTSASTOKEN_H

#include <Arduino.h>
#include <az_iot_hub_client.h>
#include <az_span.h>
#include "parametros.h"

class AzIoTHubTokenManager
{
public:
  AzIoTHubTokenManager(az_iot_hub_client* client);
  int Generate(unsigned int expiryTimeInMinutes);
  bool IsExpired();
  bool IsCreated();
  const char* Get();

private:
  az_iot_hub_client* client;
  az_span deviceKey;
  az_span signatureBuffer;
  az_span sasTokenBuffer;
  az_span sasToken;
  uint32_t expirationUnixTime;
};

#endif
