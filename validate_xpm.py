import re
import sys
import shutil

# Read main.cpp mappings
with open('src/main.cpp', 'r') as f:
    cpp_content = f.read()

mapping_match = re.search(r'CC_MAPPING MAP\[128\] = \{(.*?)\};', cpp_content, re.DOTALL)
if not mapping_match:
    print("Could not find CC_MAPPING in main.cpp")
    sys.exit(1)

mappings = {}
for line in mapping_match.group(1).split('\n'):
    line = line.strip()
    if not line or line.startswith('//'): continue
    m = re.match(r'\{(?:SYSEX|CC|SYSTEM|SKIP),\s*(\d+),\s*(\d+),\s*(\d+)', line)
    if m:
        cc = int(m.group(1))
        min_v = int(m.group(2))
        max_v = int(m.group(3))
        mappings[cc] = (min_v, max_v)

# Read TX81z-txsyx.xpm
xpm_path = 'AddOns/txSex/TX81z-txsyx.xpm'
with open(xpm_path, 'r') as f:
    xpm_lines = f.readlines()

mismatches = []
fixes_made = 0
new_xpm_lines = []

for i, line in enumerate(xpm_lines):
    new_line = line
    if '<Parameter Index="' in line:
        m = re.search(r'<Parameter Index="(\d+)" Name="([^"]+)"/>', line)
        if m:
            index = int(m.group(1))
            name = m.group(2)
            
            if len(name) > 18:
                mismatches.append(f"Line {i+1} [Index {index}]: Name '{name}' is {len(name)} chars (MAX 18)")
            
            # Match formats like " 0-127", " 0~127"
            range_match = re.search(r'\s(\d+)[\-\~](\d+)$', name)
            if range_match:
                n_min = int(range_match.group(1))
                n_max = int(range_match.group(2))
                
                if index in mappings:
                    c_min, c_max = mappings[index]
                    if n_min != c_min or n_max != c_max:
                        # Construct new name
                        base_name = name[:range_match.start()]
                        new_name = f"{base_name} {c_min}-{c_max}"
                        
                        mismatches.append(f"Fixed Line {i+1} [Index {index}]: '{name}' -> '{new_name}'")
                        new_line = line.replace(f'Name="{name}"', f'Name="{new_name}"')
                        fixes_made += 1
                        
    new_xpm_lines.append(new_line)

if mismatches:
    print("MISMATCHES/FIXES in XPM:")
    for m in mismatches:
        print(m)
    
    if fixes_made > 0:
        # Backup original just in case
        shutil.copyfile(xpm_path, xpm_path + '.bak')
        with open(xpm_path, 'w') as f:
            f.writelines(new_xpm_lines)
        print(f"\nSuccessfully wrote {fixes_made} fixes to {xpm_path}")
        print("A backup was created at TX81z-txsyx.xpm.bak")
else:
    print("XPM validation passed! All ranges match and lengths <= 18.")
