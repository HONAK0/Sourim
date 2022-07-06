from os import listdir, rename, remove

found = False
lastname = ""

for path in listdir():
	if path.endswith("-build.iso"):
		found = True
		lastname = path

if not found:
	rename("build.iso", "Sourim-1-build.iso")
else:
	remove(lastname)
	rename("build.iso", f"Sourim-{int(lastname.replace('Sourim-','').replace('-build.iso', ''))+1}-build.iso")
