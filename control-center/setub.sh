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
 echo "Usage: setub.sh --nogui -- for run install script in not GUI mode"
 exit -1
fi


  PATH_FILE=$HOME
  export SCT_CONF_FILE=$HOME/.SCT

echo -en "\033[36;1mCopyright @2007 SKsoft Group. All rights reserved.\33[0m"
echo
echo -en "\033[33;1mStarting install script...\33[0m\n"
echo


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

tar -xf data.tar
wait

echo -en "\033[33;1mBuilding project ...\33[0m\n"

echo "# run qmake ..." ; 
qmake;
if [ "$?" != 0 ] ;
then
removes_tmp;
removes_tmp1;
echo -en "\033[31;1mError building project. (qmake error).\33[0m\n";
return 2;
fi


make; 

if [ "$?" != 0 ] ;
then
echo -en "\033[31;1mError building project. (make error).\33[0m\n"
removes_tmp;
removes_tmp1;
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
removes_tmp;
removes_tmp1;
return 2;
fi
}

function create_dirs()
{
echo -en "\033[33;1mCreating directoryes ...\33[0m\n"
mkdir -p "$PATH_FILE"
mkdir -p "$PATH_FILE/bin"
mkdir -p "$PATH_FILE/lang"
mkdir -p "$PATH_FILE/modules"
mkdir -p "$PATH_FILE/pixmaps"
mkdir -p "$PATH_FILE/share"

if [ "$?" != 0 ] ;
then
echo -en "\033[31;1mError. Please check permission.\33[0m\n"
removes_tmp;
removes_tmp1;
return 2;
fi
}

function install_bin()
{
echo -en "\033[33;1mInstalling ...\33[0m\n"
  cp --force bin/* "$PATH_FILE/bin";
  cp --force -R doc "$PATH_FILE";
  cp --force lang/*.qm "$PATH_FILE/lang";
  cp --force pixmaps/*.* "$PATH_FILE/pixmaps";

if [ "$?" != 0 ] ;
then
echo -en "\033[31;1mError. Please check permission.\33[0m\n"
removes_tmp;
removes_tmp1;
return 2;
fi
}

function removes_tmp()
{
  echo -en "\033[33;1mRemoves temporary data ...\33[0m\n" 
  rm -d -R -f bin
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
rm -d -R -f doc
rm -d -R -f lang
rm -d -R -f modules
rm -d -R -f pixmaps
rm -d -R -f src

rm -f sct.pro
}

function load_kde_ui()
{
 kdialog --title "WARNING!!!" --textbox  $FILE 400 250;
   if [ "$?" -eq 0 ]
   then
     echo "Load GUI..."
   else
     load_konsole_ui;
     exit 0
   fi      
   
   kdialog --yesno "Continue installation?" 250 250;
   
   if [ $? = 1 ];
   then
   kdialog --error "Process aborting";
      exit
   fi
   path_file=`kdialog --title="Install directory" --getexistingdirectory  "$PATH_FILE/"`
   case $? in
   0)
        PATH_FILE="$path_file/SCT";;
   1)
        exit;;
   esac

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
	dcop $dcopRef close   
        exit;
    fi
    
    dcop $dcopRef setProgress 70 
    dcop $dcopRef setLabel "Installing ..." ;
    create_dirs;
    if [ "$?" = 2 ] ;
    then
       kdialog --error "Error installing. Please check permission.";
       dcop $dcopRef setLabel "Error"
       dcop $dcopRef close   
       exit;
    fi

    install_bin;
    if [ "$?" = 2 ] ;
    then
      kdialog --error "Error installing. Please check permission.";
      dcop $dcopRef setLabel "Error"
      dcop $dcopRef close   
      exit
    fi
    #sleep 1;
    dcop $dcopRef setProgress 90
    dcop $dcopRef setLabel "Removes temporary data ..." ; removes_tmp;
    dcop $dcopRef setProgress 100
    dcop $dcopRef setLabel "Finish";
    
    dcop $dcopRef close   

    if [ "$?" = 1 ] ; then
        kdialog --error "Install abort."
    fi
     
}

function load_gnome_ui()
{
   zenity --text-info --title="WARNING!!!" --filename=$FILE 400 250;
   
   if [ "$?" -eq 0 ]
   then
   echo "Load GUI..."
   else
   load_konsole_ui;
   exit 0       
   fi      
   
   zenity --question --text="Continue installation?" 250 250;

   if [ $? = 1 ];
   then
   zenity --info --text="Process aborting";
     exit
   fi

   path_file=`zenity --file-selection --filename="$PATH_FILE/" --directory --title="Install directory"`
   case $? in
   0)
           PATH_FILE="$path_file/SCT";;
   1)
           exit;;
   esac

   (
     echo "10" ;
     echo "# Preparing ..." ; 
     
     echo "20" ; 
     #building_project
     
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
     echo "75" ; 
     echo "# Installing ..." ;
     create_dirs;
     if [ "$?" = 2 ] ;
     then
       zenity --error --text="Error installing. Please check permission.";
       echo "# Error"
       exit;
     fi
     
     install_bin;
     if [ "$?" = 2 ] ;
     then
       zenity --error --text="Error installing. Please check permission.";
       echo "# Error"
       exit
     fi
     sleep 1;
     echo "90" ;
     echo "# Removes temporary data ..." ; removes_tmp;
     echo "100" ;
     echo "# Finish";
    ) |
    zenity --progress "Preparing install..." --title="Install progress" --pulsate

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
  exit
fi
  
echo -en "Install directory [$PATH_FILE/SCT/]:"
read path

if [[ "$path" = "" ]];
then
  path=$PATH_FILE
else
  PATH_FILE=${path%/}
fi

PATH_FILE="$PATH_FILE/SCT"

# building project
building_project;
if [ "$?" = 2 ]
then
  exit
fi

create_dirs;
if [ "$?" = 2 ]
then
  exit
fi

install_bin;
if [ "$?" = 2 ]
then
   exit
fi

removes_tmp;
}

chek_QT;

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
load_kde_ui;
#load_gnome_ui;
#load_konsole_ui;
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

if [ "$?" = 2 ]
then
  exit
fi

mkdir -p $SCT_CONF_FILE;

# Cretes config filename
cat > "$SCT_CONF_FILE/sctrc" <<End-of-message
[General]
Install_Dir=$PATH_FILE
ViewMode=0
End-of-message


# Write data to file
cat > "$PATH_FILE/sct" <<End-of-message
#!/bin/bash
#This script load Qt config current user
function set_qt()
{
  local user=\"\$USER\"
  if [ \"\$KDESU_USER\" ]; then
    user=\"\$KDESU_USER\"
  fi
  temp_QT_HOME_DIR=\"\`eval echo ~\$user/.qt/\`\"
  if test -r \"\$temp_QT_HOME_DIR/qtrc\"; then
   export QT_HOME_DIR=\"\$temp_QT_HOME_DIR\"
  fi
}
set_qt
$PATH_FILE/bin/sct_center
End-of-message


 
chmod a+x "$PATH_FILE/sct"


# Create desktop file
cat > "$HOME/Desktop/sct.desktop" <<End-of-message
[Desktop Entry]
Comment=control center for linux
Exec=$PATH_FILE/sct
Icon=$PATH_FILE/pixmaps/tux_config.png
MimeType=
Name=SCT
Path=
StartupNotify=true
Terminal=false
TerminalOptions=
Type=Application
X-DCOP-ServiceType=none
X-KDE-SubstituteUID=true
X-KDE-Username=
End-of-message

removes_tmp1

echo -en "\033[37;1;42mFINISH!\33[0m\n"
exit 0
#
