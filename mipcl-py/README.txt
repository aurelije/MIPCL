Welcome to MIPCL-PY
=======================

MIPCL-PY consists of a Python module (called mipshell) and a shared library (mipcl.so).
All information on MIPCL and MIPCL-PY is available on www.mipcl-cpp.appspot.com.


Installing MIPCL-PY
---------------

1. Download a mipcl-py archive from www.mipcl-cpp.appspot.com.
   The site provides download links for all supported development platforms.

2. Unpack the mipcl-py archive (tar.gz package) into your <install_dir>
  (for example, <install_dir>=/usr/local, <install_dir>=/opt,
   or any subdirectory of your home directory).

3. Enter the mipshell directory
      cd <install_dir>/mipcl-py/mipshell
   If you use Python3, type
      ln -s mipcl-py3.so mipcl.so
   If you use Python2, type
      ln -s mipcl-py2.so mipcl.so

4. Modify the PYTHONPATH environment variable
        to include the path to the MIPCL-PY module.

        On LINUX computers, if you use bash, add the following line to the end of .bash_profile:
        PYTHONPATH=<install_dir>/mipcl-py/mipshell:<install_dir>/mipcl-py/models; export PYTHONPATH

        Warning: ~/.bash_profile or ~/.profile files are not executed
                 when logging in into a Wayland session.

5. (Optional) If you have installed MIPCL-PY into a directory not available for writing,
    copy (or, if you the only developer using MIPCL-PY, move)
    the `tests` directory to any place in your home directory.

