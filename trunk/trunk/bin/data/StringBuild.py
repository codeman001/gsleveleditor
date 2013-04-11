# This Python file uses the following encoding: utf-8
#
# Utility to convert strings from game2D's XML format to Excel and back.
#
# C.D. July 2006
#
import os
import re
import os.path
import xml.dom.minidom
from pyExcelerator import *


# stuff to handle text encodings
import codecs
enc, dec, read_wrap, write_wrap = codecs.lookup('utf-8')
	
#
#
#

def parseXLSStrings( inputfile ):   
	sections = {}

	sheets = []
	sheets = parse_xls(inputfile)			
		
	for sheet in sheets :
		sectionName,vals = sheet

		#search language row and id col
		langRow = 0
		idCol = 0
		maxRow = 0
		maxCol = 0
		
		for v in vals :
			if vals[(v[0],v[1])] == '::id::':
				langRow = v[0]
				idCol = v[1]
			if v[0] > maxRow :
				maxRow = v[0]
			if v[1] > maxCol :
				maxCol = v[1]
		  
		maxCol = maxCol + 1
		maxRow = maxRow + 1
	  
		langs = []
		numLangs = 0	
	  
		#count languages and count strings		
		for col in range(maxCol) :
			if col <> idCol :
				if vals.has_key((langRow, col)) :		 
					langs.append(vals[(langRow, col)])
					langs[-1] = langs[-1].replace('::', '')
					numLangs += 1
	  
		sections[sectionName] = { 'name' : sectionName, 'ids' : [] }
		for lang in langs :
			sections[sectionName][lang] = []
	 
		for row in range(maxRow) :
			if row <> langRow :
				if vals.has_key((row, idCol)) :
					section = sections[sectionName]
					strId = vals[(row, idCol)]
					section['ids'].append(strId)	  
					for col in range(numLangs) :
						strn = ' '
						if vals.has_key((row, col + idCol + 1)) :
							strn = unicode(vals[(row, col + idCol + 1)])
							strn = strn.replace( u'\u2026' ,'...')
							strn = strn.rstrip()
						section[langs[col]].append( strn )  
		  
	return ( list(langs), sections.values() )
	       

def encode_for_xml(unicode_data, encoding='ascii'):
    """
    Encode unicode_data for use as XML or HTML, with characters outside
    of the encoding converted to XML numeric character references.
    """
    try:
        return unicode_data.encode(encoding, 'xmlcharrefreplace')
    except ValueError:
        # ValueError is raised if there are unencodable chars in the
        # data and the 'xmlcharrefreplace' error handler is not found.
        # Pre-2.3 Python doesn't support the 'xmlcharrefreplace' error
        # handler, so we'll emulate it.
        return _xmlcharref_encode(unicode_data, encoding)

def _xmlcharref_encode(unicode_data, encoding):
    """Emulate Python 2.3's 'xmlcharrefreplace' encoding error handler."""
    chars = []
    # Step through the unicode_data string one character at a time in
    # order to catch unencodable characters:
    for char in unicode_data:
        try:
            chars.append(char.encode(encoding, 'strict'))
        except UnicodeError:
            chars.append('&#%i;' % ord(char))
    return ''.join(chars)

def stringsXLS2XML(inputfile, outputfile) :

	(langs,strings) = parseXLSStrings(inputfile)

	dom = xml.dom.minidom.parseString( "<strings/>" )
	doc = dom.documentElement

	groups = ['ids'] + langs

	for section in strings :
		sectionName = section['name']
		sectionNode = dom.createElement('table')
		sectionNode.setAttribute('name',sectionName)
		doc.appendChild(sectionNode)

		# dump groups
		for group in groups :
			groupSection = dom.createElement(group)
			sectionNode.appendChild(groupSection)
			for item in section[group] :
				if group == 'ids' : 
					itemnode = dom.createElement( 'id' )
					itemnode.setAttribute('value',item)
				else :
					itemnode = dom.createElement( 'string' )
					item = item.replace('\\n','\n')
					item = item.replace('|',' ')
					#item = item.replace('\n','&#x0A;')
					#item = encode_for_xml(item)
					#item = item.replace('\n','&#x0A;')
					#item = encode_for_xml(item,'ascii')
					#item = item.replace('&amp;','&')								
					itemnode.appendChild( dom.createTextNode(item)  )
				groupSection.appendChild(itemnode)

	f = open(outputfile,'wb')
	
	tmp = dom.toprettyxml(encoding='utf-8')
	tmp = tmp.replace('&amp;','&')
	f.write( tmp )		


if __name__ == '__main__':
	if len(sys.argv) < 2:
		print "Usage: StringBuild.py infile outfile"
	else:
		infile 			= sys.argv[1]
		outfile 		= sys.argv[2]
					
		stringsXLS2XML(infile,outfile)		

		print 'Done'
