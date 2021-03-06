Build
=====

Install the `openvcdiff` packages.
Configure nginx like this:

```
./configure --add-module=/path/to/sdch_module
```

Directives
=========

sdch
----
**syntax:** *sdch (on|off);*

**default:** *off*

**context:** *location*

on – Enable module

off – Disable module

sdch_group
----------
**syntax:** *sdch_group &lt;group&gt;*

**default:** *default*

**context:** *location, server, if*

Set *group* of the query (see *sdch_dict*). *group* can contain variables 
(i.e. it is a script).

sdch_dict
---------
**syntax:** *sdch_dict &lt;filename&gt; [&lt;group&gt; [&lt;priority&gt;]]*

**context:** *location, server, if*

Specify the file with the dictionary. It is possible to configure several 
dictionaries by specifying the directive per dictionary.
If a client have several configured dictionaries, the dictionary for use is 
selected like this: 
1. A dictionary with *group*  that equal to *sdch_group* of the query is 
preferred over a dictionary those *group* is not equal to the *sdch_group* 
of the query.
2. If the *group* is equally good, lower *priority* is preferred over 
high *priority*.

sdch_url
--------
**syntax:** *sdch_url &lt;url&gt;*

**context:** *location, server, if*

Url to suggest to the client (in the `Get-Dictionary` header) if it does not 
have the best possible dictionary. Variables allowed.

sdch_disablecv
--------------
**syntax:** *sdch_disablecv &lt;script&gt;*

Disable SDCH if the value is non-empty and is not equal to 0.

sdch_min_length
--------------
**syntax:** *sdch_min_length &lt;length&gt;*

Minimal size of response to encode.

sdch_types
----------
**syntax:** *sdch_types &lt;types&gt;*

Enable encoding responses with list of mime/types. Default is text/html, text/css, application/javascript, and application/x-sdch-dictionary.

sdch_nodict_types
-----------------
**syntax:** *sdch_nodict_types &lt;types&gt;*

Don't announce a dictionary (with the Get-Dictionary header) if the response has Content-Type listed here. The default is application/x-sdch-dictionary, i.e. an SDCH dictionary.

sdch_fastdict
----------
**syntax:** *sdch_fastdict (on|off);*

**context:** *location, server, if*

**default:** *off*

Enable quasi dictionaries. I.e., if client support quasi-dictionaries, store 
the reply in memory and allow client to use the reply as a dictionary.

If a client announces several quasi-dictionaries to the server, the one with 
the group that match the group of the current query is preferred. Configured 
dictionaries, if any, is preferred over all quasi-dictionaries.

sdch_stor_size
--------------
**syntax:** *sdch_stor_size &lt;memsize&gt;*

**context:** *main*

**default:** *10 000 000*

The memory limit for quasi-dictionaries.

sdch_vary
--------------
**syntax:** *sdch_vary (on|off)*

**context:** *main, location, server*

**default:** *on*

Add Vary header

The FastDict protocol extension
===============================
To announce FastDict support, the client sends `Sdch-Features: fastdict`
header.

If the client support FastDicts, and server suggest that the current reply 
will be used as the quasi-dictionary, it add `X-Sdch-Use-As-Dictionary` 
header to the reply. 

Then, the client may use the reply as a dictionary, i.e. announce it in the 
`Avail-Dictionary` header etc.

(There is no headers in the quasi-dictionaries.)
