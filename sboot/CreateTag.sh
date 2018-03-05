function CherryPick(){
	echo CherryPick_MstarApp:[Enter to continue]
	read CHERRYTMP
	if [ "$CHERRYTMP" ];then
		TAGMESSAGE+="
CherryPick_MstarApp:$CHERRYTMP"
	fi
	echo CherryPick_MstarCore:[Enter to continue]
	read CHERRYTMP
	if [ "$CHERRYTMP" ];then
		TAGMESSAGE+="
CherryPick_MstarCore:$CHERRYTMP"
	fi
	echo CherryPick_MstarCustomer:[Enter to continue]
	read CHERRYTMP
	if [ "$CHERRYTMP" ];then
		TAGMESSAGE+="
CherryPick_MstarCustomer:$CHERRYTMP"
	fi
	echo CherryPick_Tool:[Enter to continue]
	read CHERRYTMP
	if [ "$CHERRYTMP" ];then
		TAGMESSAGE+="
CherryPick_Tool:$CHERRYTMP"
	fi
	echo CherryPick_UBoot:[Enter to continue]
	read CHERRYTMP
	if [ "$CHERRYTMP" ];then
		TAGMESSAGE+="
CherryPick_UBoot:$CHERRYTMP"
	fi
	echo CherryPick_SBoot:[Enter to continue]
	read CHERRYTMP
	if [ "$CHERRYTMP" ];then
		TAGMESSAGE+="
CherryPick_SBoot:$CHERRYTMP"
	fi
}

if [ ! "$1" ] || [ "$1" == "-c" ];then
	echo Please input a tag.
	exit
fi
if [ "$1" == "help" ];then
	echo -e Usage:
	echo -e CreateTag TAGNAME for add a simple tag
	echo -e CreateTag TAGNAME DESCRIPTION for adding a description
	echo -e CreateTag TAGNAME -c for adding cherry-pick commands
	echo -e CreateTag TAGNAME DESCRIPTION -c for adding a description and cherry-pick commands
	exit
fi
echo Tag is $1
if [ "$2" ] && [ "$2" != "-c" ];then
	echo Description is $2
	DESCRIPTION=$2
fi

cd ../MstarApp/
MstarApp_Commit=$(git rev-parse HEAD)
cd ../MstarCore/
MstarCore_Commit=$(git rev-parse HEAD)
cd ../MstarCustomer/
MstarCustomer_Commit=$(git rev-parse HEAD)
cd ../tool/
tool_Commit=$(git rev-parse HEAD)
cd ../u-boot-2011.06/
uboot_Commit=$(git rev-parse HEAD)
cd ../sboot/
sboot_Commit=$(git rev-parse HEAD)

TAGMESSAGE="Created by $USER
Description: $DESCRIPTION
MstarApp: $MstarApp_Commit
MstarCore: $MstarCore_Commit
MstarCustomer: $MstarCustomer_Commit
Tool: $tool_Commit
UBoot: $uboot_Commit
SBoot: $sboot_Commit
Date: $(date '+%Y/%m/%d %H:%M:%S')"
if [ "$2" == "-c" ] || [ "$3" == "-c" ];then
	TAGMESSAGE+="
-----------------------------"
	CherryPick;
fi
echo "--------------------------------------------------"
cd ../MstarApp/
git tag -a $1 -m "$(git remote -v | head -n1 | awk '{print $2}' | sed 's/.*\///' | sed 's/\.git//')
$TAGMESSAGE"
if [ "$?" -ne "0" ];then
	exit 1
fi
cd ../MstarCore/
git tag -a $1 -m "$(git remote -v | head -n1 | awk '{print $2}' | sed 's/.*\///' | sed 's/\.git//')
$TAGMESSAGE"
if [ "$?" -ne "0" ];then
    exit 1
fi
cd ../MstarCustomer/
git tag -a $1 -m "$(git remote -v | head -n1 | awk '{print $2}' | sed 's/.*\///' | sed 's/\.git//')
$TAGMESSAGE"
if [ "$?" -ne "0" ];then
    exit 1
fi
cd ../tool/
git tag -a $1 -m "$(git remote -v | head -n1 | awk '{print $2}' | sed 's/.*\///' | sed 's/\.git//')
$TAGMESSAGE"
if [ "$?" -ne "0" ];then
    exit 1
fi
cd ../u-boot-2011.06/
git tag -a $1 -m "$(git remote -v | head -n1 | awk '{print $2}' | sed 's/.*\///' | sed 's/\.git//')
$TAGMESSAGE"
if [ "$?" -ne "0" ];then
    exit 1
fi
cd ../sboot/
git tag -a $1 -m "$(git remote -v | head -n1 | awk '{print $2}' | sed 's/.*\///' | sed 's/\.git//')
$TAGMESSAGE"
if [ "$?" -ne "0" ];then
    exit 1
fi
repo forall -c "git tag $1 -l -n20"
