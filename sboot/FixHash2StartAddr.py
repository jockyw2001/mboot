#FixHash2StartAddr.py
import sys

MMAP_256MB = 0
MMAP_128MB = 0
MMAP_64MB = 0

configFileSource = sys.argv[1]
configFile = open(configFileSource.rstrip(),'r+') 

#CPU_type: arm/mips
CPU_type = sys.argv[2]

if CPU_type == 'mips':
    #Reserve the last 128KB space of 256MB/128MB/64MB mmap for loader(DC on) for MIPS series chips(keres series, k1p, k5tn)
	Hash2StartAddr_256MB = '0x8FFE0000'
	Hash2StartAddr_128MB = '0x87FE0000'
	Hash2StartAddr_64MB = '0x83FE0000'
elif CPU_type == 'arm':
	#Reserve the last 512KB space of 256MB/128MB/64MB mmap for loader(DC on) for ARM series chips(k6, k6lite, k7)
	Hash2StartAddr_256MB = '0x2FF80000'
	Hash2StartAddr_128MB = '0x27F80000'
	Hash2StartAddr_64MB = '0x23F80000'
else:
	print 'Wrong argument(CPU type: mips/arm)!'

while True :
	fileString = configFile.readline()
	
	if (fileString.strip() == 'CONFIG_MSTAR_MMAP_512MB=y') or (fileString.strip() == 'CONFIG_MSTAR_MMAP_256MB=y') or (fileString.strip() == 'CONFIG_MSTAR_MMAP_1GB_512MB=y') or (fileString.strip() == 'CONFIG_MSTAR_MMAP_512MB_512MB=y') or (fileString.strip() == 'CONFIG_MSTAR_MMAP_256MB_256MB=y'):
		print 'MMAP_256MB' 
		MMAP_256MB = 1
		break
	elif (fileString.strip() == 'CONFIG_MSTAR_MMAP_128MB=y') or (fileString.strip() == 'CONFIG_MSTAR_MMAP_128MB_128MB=y') or (fileString.strip() == 'CONFIG_MSTAR_MMAP_128MB_64MB=y'):
		MMAP_128MB = 1
		print 'MMAP_128MB'
		break		
	elif (fileString.strip() == 'CONFIG_MSTAR_MMAP_64MB=y') or (fileString.strip() == 'CONFIG_MSTAR_MMAP_64MB_64MB=y') or (fileString.strip() == 'CONFIG_MSTAR_MMAP_64MB_64MB=y'):
		MMAP_64MB = 1
		print 'MMAP_64MB' 
		break
	elif fileString == '':
		print 'No appropriate setting of mmap size in .config file!'
		break
			
configFile = open(configFileSource,'r+') 	
StringFilter = 'CONFIG_HASH2_START_ADDRESS'
fileaAll = configFile.read()
StringPosition = fileaAll.find(StringFilter) 
configFile.seek(StringPosition,0)

if MMAP_256MB == 1 :
	configFile.write('CONFIG_HASH2_START_ADDRESS=' + Hash2StartAddr_256MB)  	
	configFile.close()		
elif MMAP_128MB == 1:
	configFile.write('CONFIG_HASH2_START_ADDRESS=' + Hash2StartAddr_128MB)  	
	configFile.close()	
elif (MMAP_64MB == 1):
	configFile.write('CONFIG_HASH2_START_ADDRESS=' + Hash2StartAddr_64MB)
	configFile.close()	
else:
	print 'No appropriate setting of mmap size in .config file!'	

print('Fix hash2 start address done.\n')
