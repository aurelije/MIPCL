Welcome to MIPCL-PY
=======================

MIPCL-PY consists of a Python module (called mipshell) and a shared library (mipcl.so).
All information on MIPCL and MIPCL-PY is available on www.mipcl-cpp.appspot.com.


Installing MIPCL-PY
---------------

1. Download a mipcl-py archive from www.mipcl-cpp.appspot.com.
   The site provides download links for all supported development platforms.

2. Unpack the mipcl-py archive (tar.gz package) into your installation directory.
  (for example, any subdirectory of your home directory, or /usr/local, /opt).

3. Enter the mipshell directory
      cd <install_dir>/mipcl_py/mipshell
   If you use Python3, type
      ln -s mipcl-py3.so mipcl.so
   If you use Python2, type
      ln -s mipcl-py2.so mipcl.so

Here <install_dir> refers to the directory that contains the "mipcl_py" directory.

4. Modify the PYTHONPATH environment variable
        to include the path to the MIPCL-PY module and examples.

        On LINUX computers, if you use bash, add the following line to the end of .bashrc:
        PYTHONPATH=<install_dir>; export PYTHONPATH

5. (Optional) If you have installed MIPCL-PY into a directory not available for writing,
    copy (or, if you are the only developer using MIPCL-PY, move)
    the `tests` directory to any place in your home directory.


