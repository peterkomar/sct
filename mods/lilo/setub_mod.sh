#!/bin/bash
# Author:   Peter Komar
# e-mail:   udldevel@gmail.com
# Copyring: All rights reserved.
# for Qt3

FILE=license.txt

opt=$1

if [ -z $opt ]
then
opt="--gui"
fi

if [[ $opt != "--nogui" && $opt != "--gui" ]]; then
echo "Unknown options $opt"
echo "Usage: setub_mod.sh --nogui -- for run install script in not GUI mode"
exit -1
fi


export NAME_FILE=$HOME/.SCT/sctrc

echo -en "\033[36;1mCopyright @2007 SKsoft Group. All rights reserved.\33[0m"
echo
echo -en "\033[33;1mStarting install script...\33[0m\n"
echo

tar -xf data.tar
wait

function chek_QT()
{
# Checkin for Qt

echo -en "\033[33;1mCheckin for Qt ...\33[0m                       "

if [ $QTDIR ];
then
echo -en "\033[32;1mdone\33[0m\n"
else
echo -en "\033[31;1mfailed\33[0m\n"
return 2
fi

echo -en "\033[33;1mCheckin for Qt devel ...\33[0m                 "

if [ -e "$QTDIR/bin/qmake" ];
then
echo -en "\033[32;1mdone\33[0m\n"
else
echo -en "\033[31;1mfailed\33[0m\n"
return 2
fi

return 0
}

function building_project()
{
echo -en "\033[33;1mBuilding module ...\33[0m\n"
qmake;

if [ "$?" != 0 ] ;
then
echo -en "\033[31;1mError building project. (qmake error).\33[0m\n";
return 2;
fi

make

if [ "$?" != 0 ] ;
then
echo -en "\033[31;1mError building project. (make error).\33[0m\n"

  if [ $KDE_FULL_SESSION ];
  then
  {
      kdialog --error "Error building project. Run setub program in terminal for details.";
      dcop $dcopRef setLabel "Error"
      dcop $dcopRef close
      exit -1;
  }
  else
     if [ $GNOME_FULL_SESSION ];
     then
       zenity --error --text="Error building project. Run setub program in terminal for details.";
       echo "# Error"
       exit -1;
     fi
  fi

return 2;
fi

lrelease *.pro;
wait

if [ "$?" != 0 ] ;
then
echo -en "\033[31;1mError building project. (lrelease error).\33[0m\n"
return 2;
fi

}

function gets_install_path()
{

declare -i k
declare -i n

if [ -e "$NAME_FILE" ];
then    
echo "Config file is exists."
else
echo -en "\033[10;5000;11;100]\007"
echo -en "\033[37;1;41m ERROR: \33[0m"
echo -en " \033[31;1m File $NAME_FILE not found.\33[0m"
echo
echo -en "\033[36;1mYou have install SCT control center first or run (setub_mod.sh) like root\33[0m\n"
echo
echo "Process aborting ..."

return 2
fi
echo "Test config file is complete."


for name in $(awk '{print $1}' < "$NAME_FILE" )
				 do
				 k=$(expr index "$name" = );
				 if [ $k = 0 ];
				 then
				 continue
				 fi
				 let "n=$k-1"
				 tmp=${name:0:n}
				 if [[ $tmp = "Install_Dir" ]];
				 then
				 tmp=${name:k};
				 echo "$tmp"
				 break
				 fi
                                 done 
	REPLY=$tmp;
	return 0
}

function create_dirs()
{
echo -en "\033[33;1mCreating directoryes ...\33[0m\n"
gets_install_path;
path_file=$REPLY
mkdir -p "$path_file/share/lilo" #for SCT lilo
#mkdir -p "$path_file/share/rpm"  #for SCT rpm

if [ "$?" != 0 ] ;
then
echo -en "\033[31;1mError. Please check permission.\33[0m\n"
return 2;
fi
}

function install_bin()
{
create_dirs;

  gets_install_path
  tmp=$REPLY
  echo -en "\033[33;1mInstalling ...\33[0m\n"
  cp --force modules/*.so "$tmp/modules/";
  cp --force lang/*.qm "$tmp/lang/";
  cp --force pixmaps/*.png "$tmp/pixmaps/";
  cp --force -R themes "$tmp/share/lilo"; #for SCT lilo
}

function removes_tmp()
{
echo -en "\033[33;1mRemoves temporary data ...\33[0m\n" 
rm -d -R -f modules
rm -d -R -f src/.ui
rm -d -R -f src/.obj
rm -d -R -f src/.moc

rm -f Makefile
rm -f src/Makefile
rm -f lang/*.qm
}

function removes_tmp1()
{
   echo -en "\033[33;1mRemoves temporary sources...\33[0m\n" 
   rm -d -R -f lang
   rm -d -R -f pixmaps
   rm -d -R -f src
   rm -d -R -f themes #for SCT lilo

   rm -f sct.pro
}

function load_kde_ui()
{
     kdialog --title "WARNING!!!" --textbox  $FILE 400 250;

     if [ "$?" -eq 0 ]
     then
       echo ""
     else
       load_konsole_ui;
       exit 0       
     fi      

     kdialog --yesno "Continue installation?" 250 250;

     if [ $? = 1 ];
     then
          kdialog --error "Process aborting";
          exit -1
     fi

     dcopRef=`kdialog --progressbar --title="Install progress" "Preparing install..." 10`
     
     dcop $dcopRef setProgress 1
     dcop $dcopRef setLabel "Preparing ..."
     
     
     building_project & ( dcop $dcopRef setProgress 2
     sleep 1;
     #building_project
     dcop $dcopRef setProgress 30;
     dcop $dcopRef setLabel "Building ... 20%"
     sleep 1;
     dcop $dcopRef setProgress 50;
     dcop $dcopRef setLabel "Building ... 40%"
     sleep 1;
     dcop $dcopRef setProgress 60;
     dcop $dcopRef setLabel "Building ... 60%"
     sleep 2;
     dcop $dcopRef setProgress 90;
     dcop $dcopRef setLabel "Building ... 90%"
     sleep 2;
     dcop $dcopRef setProgress 99;
     dcop $dcopRef setLabel "Building ... 99%"
     );
     wait
       
       if [ "$?" = 2 ] ;
       then
          kdialog --error "Error building project. Run setub program in shell for details.";
          dcop $dcopRef setLabel "Error"
          exit;
       fi
       
       dcop $dcopRef setProgress 70 
       dcop $dcopRef setLabel "Installing ..." ; install_bin;
       
       if [ "$?" = 2 ];
       then
         kdialog --error "Please install and run (SCT control center) first. Or You have run (setub_mod.sh) like root.";
         echo "Error";
         exit -1
       fi;
       dcop $dcopRef setProgress 90 
       dcop $dcopRef setLabel "Removes temporary data ..." ;
       removes_tmp;
       dcop $dcopRef setProgress 100 
       dcop $dcopRef setLabel "Finish" ;
       
       dcop $dcopRef close   

      if [ "$?" = 1 ] ; then
           zenity --error "Install abort."
      fi
}

function load_gnome_ui()
{
    zenity --text-info --title="WARNING!!!" --filename=$FILE 400 250;

    if [ "$?" -eq 0 ]
    then
      echo ""
    else
      load_konsole_ui;
      exit 0       
    fi      

    zenity --warning --text="Continue installation?" 250 250;

   if [ $? = 1 ];
   then
    zenity --info --text="Process aborting";
    exit -1
   fi
   
   (
         building_project & ( echo "30"
                              echo "# Building ... 20%"
                              sleep 1;
                              echo "50" ;
                              echo "# Building ... 40%" ; 
                              sleep 1;
                              echo "60" ;
                              echo "# Building ... 60%" ;
                              sleep 2;
                              echo "90" ;
                              echo "# Building ... 90%" ;
                              sleep 2;
                              echo "99" ;
                              echo "# Building ... 99%" ;
                            )
   
	wait
	
	if [ "$?" = 2 ] ;
	then
	  zenity --error --text="Error building project. Run setub program in shell for details.";
	  echo "# Error"
	  exit;
        fi
	
	echo "70"
        echo "# Installing ..." ; install_bin;
        if [ "$?" = 2 ];
        then zenity --error \
                    --text="Please install and run (SCT control center) first.
                           Or You have run (setub_mod.sh) like root.";
              echo "# Error";
              exit -1
        fi;
        echo "90" ;
        echo "# Removes temporary data ..." ; removes_tmp;
        echo "100" ;
        echo "# Finish" ; 
   ) |
   zenity --progress "Preparing install..." --title="Install progress"

   if [ "$?" = 1 ] ; then
   zenity --error "Install abort."
   fi
}

function load_konsole_ui()
{


echo -en "                     \033[37;1;41mWARNING:\33[0m"
echo
echo "DISCLAIMER: THIS IS EXPERIMENTAL SOFTWARE. USE AT YOUR OWN RISK. "
echo -en "UNDER NO CIRCUMSTANCES MAY \033[36;1mSKsoft Group\33[0m BE HELD LIABLE FOR DAMAGE TO HARDWARE OR SOFTWARE, LOST DATA OR OTHER"
echo -en "DAMAGE EITHER DIRECT OR INDIRECT CAUSED BY USING THIS SOFTWARE. "
echo -en "IN SOME COUNTRIES, CRYPTOGRAPHIC SOFTWARE MAY BE SUBJECT TO IMPORT/EXPORT RESTRICTIONS OR SUBJECT TO SOFTWARE PATENTS. THIS"
echo -en "SOFTWARE MAY NOT BE USED OR DISSEMINATED IN THOSE COUNTRIES. "
echo -en "OTHERWISE, IT IS SUBJECT TO THE USUAL RULES OF THE GPL LICENSE. IF YOU DO NOT WISH TO ABIDE BY THESE RULES, YOU MAY NOT " 
echo -en "USE OR DISSEMINATE THIS SOFTWARE.\n"

echo -en "\033[33;1mYou are accept this [y/n]? (n):\33[0m"
read yes

echo

if [[ $yes != "y" && $yes != "Y" ]];
then
   echo -en "\033[33;1mProcess aborting ...\33[0m\n"
   exit -2
fi

# building project
building_project;
install_bin;
if [ "$?" = 2 ];
then
  exit -1
fi

removes_tmp;
}

chek_QT

if [ "$?" = 2 ] ;
then
     if [ $KDE_FULL_SESSION ];
     then
     {
         kdialog --error "Problem checkin Qt. Qt library or Qt devel not found";
         exit -1;
     }
     else
         if [ $GNOME_FULL_SESSION ];
         then
              zenity --error --text="Problem checkin Qt. Qt library or Qt devel not found.";
              exit -1;
         fi
     fi
fi
 

if [ "$opt" = "--nogui" ] ;
then
load_konsole_ui;
else
{
   if [ $KDE_FULL_SESSION ];
   then
      # kde running
      load_kde_ui
      #load_gnome_ui
   else
      if [ $GNOME_FULL_SESSION ];
      then
       # Gnome is running
       load_gnome_ui;
      else
      # terminal is running
      load_konsole_ui;
     fi
  fi
}
fi

removes_tmp1

echo -en "\033[37;1;42mFINISH!\33[0m\n"

exit 0
#
