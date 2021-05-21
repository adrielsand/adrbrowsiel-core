/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/ipfs/pref_names.h"

// Used to determine which method should be used to resolve ipfs:// and ipns:///
// schemes, between:
// Ask: Uses a gateway but also prompts the user with an infobar.
// Gateway: Uses a gateway without prompting the user.
// Local: Uses a local node.
// Disabled: Disables all IPFS handling.
const char kIPFSResolveMethod[] = "adrbrowsiel.ipfs.resolve_method";

// Stores the location of the IPFS binary
const char kIPFSBinaryPath[] = "adrbrowsiel.ipfs.binary_path";

// Used to determine whether to automatically fallback to gateway when the
// local node is not available.
const char kIPFSAutoFallbackToGateway[] = "adrbrowsiel.ipfs.auto_fallback_to_gateway";

// Used to automatically redirect Gateway resources with x-ipfs-path
// header to the configured adrbrowsiel IPFS gateway.
const char kIPFSAutoRedirectGateway[] = "adrbrowsiel.ipfs.auto_redirect_gateway";

// Used to automatically redirect for DNSLink resources
const char kIPFSAutoRedirectDNSLink[] = "adrbrowsiel.ipfs.auto_redirect_dnslink";

// The number of times the infobar is shown to ask the user to install IPFS
const char kIPFSInfobarCount[] = "adrbrowsiel.ipfs.infobar_count";

// The number of storage used by IPFS Node
const char kIpfsStorageMax[] = "adrbrowsiel.ipfs.storage_max";

// Used to enable/disable IPFS via admin policy.
const char kIPFSEnabled[] = "adrbrowsiel.ipfs.enabled";

// Stores IPFS public gateway address to be used when translating IPFS URLs.
const char kIPFSPublicGatewayAddress[] = "adrbrowsiel.ipfs.public_gateway_address";
