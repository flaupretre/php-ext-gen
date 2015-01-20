# Installing a <i>php-ext-gen</i> extension

Compiling and installing a php-ext-gen extension requires one more step than 'traditional' PECL extensions : the C source files must first be generated.

Follow these steps :

* If it is not already present on the host, download the php-ext-gen software from
[the php-ext-gen repository](https://github.com/flaupretre/php-ext-gen/releases). Choose the latest release.

* Uncompress/untar the tgz file anywhere you want. In the base directory, you will find a file name 'php-ext-gen.phk'. This is the php-ext-gen software. The rest of the distrib is made up of examples and documentation, you can discard them if you want. The only important part is the 'php-ext-gen.phk' file. You can move it anywhere you want. In the rest of the document, we'll consider it is located in /usr/local/etc.

* cd to the directory containing your extension's source files.

* Run:

		php /usr/local/etc/php-ext-gen.phk build

	(prefix 'php' with the right path if it is not in your PATH)

* This command will generate and populate a subdirectory named 'gen'. This directory contains the 'ready-to-compile' extension. Note that php-ext-gen does not write anything outside of the 'gen' subdirectory. Cleanup is done by removing the 'gen' subdir.

* cd to the 'gen' subdirectory.

That's all. Now, you can proceed exactly as with any other PECL extension ('phpize', 'configure',...).
