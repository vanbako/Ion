#define MyRS1 \
"RootFlags( " \
  "ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | " \
  "DENY_HULL_SHADER_ROOT_ACCESS | " \
  "DENY_DOMAIN_SHADER_ROOT_ACCESS | " \
  "DENY_GEOMETRY_SHADER_ROOT_ACCESS), " \
"DescriptorTable(CBV(b0, numDescriptors = 1)), " \
"DescriptorTable(CBV(b1, numDescriptors = 1))"
