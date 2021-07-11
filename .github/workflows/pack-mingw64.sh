#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage:"
    echo "    ./package_mingw64.sh srain_install_dir tmp_dir_to_package output_package.zip"
    exit 1
fi

function get_dll() {
    ret=""
    for var in "$@"
    do
        echo "DLLs used by $var:"
        echo `ldd $var`
        ret+="$(ldd $var | tr '=>'  '\n'| grep -o '/mingw64.*\.dll' | sort -u) "
    done
}

prefix=$MINGW_PREFIX

install_dir=$1
srain_exe=$install_dir/bin/srain.exe
srain_etc=$install_dir/etc/srain
srain_share=$install_dir/share/*

tmp_dir=$2
dst_bin=$tmp_dir
dst_etc=$tmp_dir/etc
dst_lib=$tmp_dir/lib
dst_share=$tmp_dir/share

output_file=$3

# gdbus
gdbus=$prefix/bin/gdbus.exe

# gspawn-win32-helper
gspawn_win32_helper=$prefix/bin/gspawn-win32-helper.exe

# srain
cp -vf $srain_exe $prefix/bin/srain.exe # make DLLs of mingw64 have the highest priority
get_dll $prefix/bin/srain.exe
mingw64_dlls=$ret

# glib-networking
libgio_modules=$(ls $prefix/lib/gio/modules/lib*.dll)
get_dll $libgio_modules
mingw64_dlls+=$ret

# gdbus
get_dll $gdbus
mingw64_dlls+=$ret

# gspawn-win32-helper
get_dll $gspawn_win32_helper
mingw64_dlls+=$ret

mingw64_dlls=$(echo $mingw64_dlls | tr ' ' '\n' | sort -u)

mkdir -pv $dst_bin
mkdir -pv $dst_lib
mkdir -pv $dst_etc
mkdir -pv $dst_share

# portable installation of srain
cp -rfv $srain_exe $dst_bin/srain.exe
cp -rfv $srain_etc $dst_etc/
cp -rfv $srain_share $dst_share/
mkdir -pv $dst_bin/logs
touch $dst_bin/srain.cfg

echo 'Finished installing Srain'

cp -rfv $gdbus $dst_bin/
cp -rfv $gspawn_win32_helper $dst_bin/
cp -rfv $mingw64_dlls $dst_bin/
cp -rfv $prefix/lib/gio $dst_lib/
cp -rfv $prefix/lib/gdk-pixbuf-2.0 $dst_lib/
cp -rfv $prefix/share/icons $dst_share/
cp -rfv $prefix/share/glib-2.0 $dst_share/

echo 'Finished installing DLLs required by Srain'

# Remove icon-theme.cache to fix incorrect icon showed in the taskbar
rm -f $dst_share/icons/*/icon-theme.cache

zip -r $output_file $tmp_dir

echo 'Done'
