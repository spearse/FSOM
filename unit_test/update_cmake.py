import os
import sys

g_templateVars = {
	'target' : 'unit_test',
	'libs' : 'fsom'
}

g_cmakeHeader="""########## Auto Updated From Python ###########

cmake_minimum_required(VERSION 2.8)

""".format(**g_templateVars)

g_cmakeFooter="""add_executable({target} ${{{target}_SOURCES}})
target_link_libraries({target} {libs})
""".format(**g_templateVars)

g_testFiles = []
for root, subFolders, files in os.walk('src'):
	for f in files:
		if f.endswith('.cpp') or f.endswith('.h') or f.endswith('.hpp'):
			path = root+'/'+ f
			print path
			g_testFiles.append(path)

with open('CMakeLists.txt','w') as cmake:
	cmake.write(g_cmakeHeader)

	sourcesString = "set ({target}_SOURCES\n".format(**g_templateVars);
	for f in g_testFiles:
		sourcesString += "\t{0}\n".format(f)
	sourcesString += ")\n\n"

	cmake.write(sourcesString)
	cmake.write(g_cmakeFooter)

