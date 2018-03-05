function RunCherryPick(){
	for ((index=1;index<100; index++)); do
    command=$(echo $CMDS | awk -F";" -v var=$index '{print $var}')
		if [ ! "$command" ];then
			break;
		fi
		echo $command
		$command
	done
}

if [ ! "$1" ];then
    echo Please input a tag.
	exit
fi

cd ../MstarApp
CMDS=$(git tag -l $1 -n100 | grep CherryPick_MstarApp | sed 's/\ CherryPick_MstarApp://')
CMDS=$(echo $CMDS | sed 's/\ git//')
echo --------------------------------------
echo CherryPick MstarApp
RunCherryPick;
echo --------------------------------------

cd ../MstarCore
CMDS=$(git tag -l $1 -n100 | grep CherryPick_MstarCore | sed 's/\ CherryPick_MstarCore://')
CMDS=$(echo $CMDS | sed 's/\ git//')
echo --------------------------------------
echo CherryPick MstarCore
RunCherryPick;
echo --------------------------------------

cd ../MstarCustomer
CMDS=$(git tag -l $1 -n100 | grep CherryPick_MstarCustomer | sed 's/\ CherryPick_MstarCustomer://')
CMDS=$(echo $CMDS | sed 's/\ git//')
echo --------------------------------------
echo CherryPick MstarCustomer
RunCherryPick;
echo --------------------------------------

cd ../tool
CMDS=$(git tag -l $1 -n100 | grep CherryPick_Tool | sed 's/\ CherryPick_Tool://')
CMDS=$(echo $CMDS | sed 's/\ git//')
echo --------------------------------------
echo CherryPick Tool
RunCherryPick;
echo --------------------------------------

cd ../u-boot-2011.06
CMDS=$(git tag -l $1 -n100 | grep CherryPick_UBoot | sed 's/\ CherryPick_UBoot://')
CMDS=$(echo $CMDS | sed 's/\ git//')
echo --------------------------------------
echo CherryPick UBoot
RunCherryPick;
echo --------------------------------------

cd ../sboot
CMDS=$(git tag -l $1 -n100 | grep CherryPick_SBoot | sed 's/\ CherryPick_SBoot://')
CMDS=$(echo $CMDS | sed 's/\ git//')
echo --------------------------------------
echo CherryPick SBoot
RunCherryPick;
echo --------------------------------------
