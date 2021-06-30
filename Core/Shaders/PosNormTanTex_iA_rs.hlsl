#define main \
"RootFlags( " \
  "ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | " \
  "DENY_HULL_SHADER_ROOT_ACCESS | " \
  "DENY_DOMAIN_SHADER_ROOT_ACCESS | " \
  "DENY_GEOMETRY_SHADER_ROOT_ACCESS), " \
"DescriptorTable(CBV(b0, numDescriptors = 1), visibility = SHADER_VISIBILITY_ALL), " \
"DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
"SRV(t9), " \
"StaticSampler(s0, Filter = FILTER_MIN_MAG_MIP_LINEAR, AddressU = TEXTURE_ADDRESS_WRAP, AddressV = TEXTURE_ADDRESS_WRAP, AddressW = TEXTURE_ADDRESS_WRAP)"
