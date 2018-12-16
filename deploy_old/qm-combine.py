#!/usr/bin/env python3
# Usage: qm-combine.py [--deploy-qt] <out_tr_dir> <in.tsdict> <lconvert|lrelease>...

import sys
import argparse
import os.path
import glob
import tempfile
import copy
import subprocess
import shutil
try:
	from defusedxml.ElementTree import parse, ElementTree, Element
except ImportError:
	from xml.etree.ElementTree import parse, ElementTree, Element


class QmCombine:
	in_file: str
	out_dir: str
	tr_dir: str
	deploy: bool
	tool_args: list

	tmpdir: tempfile.TemporaryDirectory
	fname: str

	def __init__(self, in_file: str, out_dir: str, tr_dir: str, deploy: bool, tool_args: list):
		self.in_file = in_file
		self.out_dir = out_dir
		self.tr_dir = tr_dir
		self.deploy = deploy
		self.tool_args = tool_args
		#print(in_file, out_dir, deploy, tool_args)

	def create_qm_files(self):
		with tempfile.TemporaryDirectory() as tmpdir:
			self.tmpdir = tmpdir
			self.fname = os.path.splitext(os.path.basename(self.in_file))[0]

			xml_in = parse(self.in_file)
			root = xml_in.getroot()
			depends = root.find("dependencies")
			for locale in self.collect_locales(depends.get("root")):
				self.create_qm_metafile(copy.deepcopy(xml_in), locale)

	def collect_locales(self, root: str) -> list:
		locales = []
		basepath = os.path.join(self.out_dir, root)
		for file in glob.glob(basepath + "_*.qm"):
			locales.append(file[len(basepath)+1:-3])
		return locales

	def create_qm_metafile(self, tsdict: ElementTree, locale: str):
		tsroot = tsdict.getroot()
		tsroot.tag = "TS"
		deproot = tsroot.find("dependencies")
		deproot.attrib.pop("root")
		for elem in deproot.findall("dependency"):
			catalog = elem.get("catalog")
			if self.check_and_deploy(catalog, locale):
				elem.set("catalog", catalog + "_" + locale)
			else:
				deproot.remove(elem)

		ts_file = os.path.join(self.tmpdir, self.fname + "_" + locale + ".ts")
		tsdict.write(ts_file, encoding="UTF-8", xml_declaration=True)
		subprocess.run(self.tool_args + [
			ts_file, "-qm", os.path.join(self.out_dir, self.fname + "_" + locale + ".qm")
		], check=True)

	def check_and_deploy(self, qmbase: str, locale: str) -> bool:
		filename = qmbase + "_" + locale + ".qm"
		dep_name = os.path.join(self.out_dir, filename)
		qt_name = os.path.join(self.tr_dir, filename)
		if os.path.isfile(dep_name):
			return True
		elif os.path.isfile(qt_name):
			if self.deploy:
				print("Copying Qt-Translation for deployment:", filename)
				shutil.copy2(qt_name, dep_name)
			return True
		else:
			return False

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description="Convert an ts-dict-file to the combined translation file or a meta file")
	parser.add_argument("--deploy-qt", dest="deploy", action="store_true", help="Deploy Qt-QM-Files")
	parser.add_argument("tr_dir", metavar="qt_tr_dir", help="The directory where qt translations are stored")
	parser.add_argument("in_file", metavar="in.tsdict", help="The tsdict file to use to generate the target")
	parser.add_argument("out_dir", metavar="out_tr_dir", help="The directory where translations are deployed to")
	parser.add_argument("tool_args", metavar="lrelease-args", nargs="+", help="The tsdict file to use to generate the target")
	res = parser.parse_args(sys.argv[1:])

	combine = QmCombine(res.in_file, res.out_dir, res.tr_dir, res.deploy, res.tool_args)
	combine.create_qm_files()
