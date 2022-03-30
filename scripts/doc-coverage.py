#!/usr/bin/env python3
from argparse import ArgumentParser, Namespace
import logging
from math import floor
from os import path
from pprint import pprint
import re
import sys
from typing import Dict, List, NamedTuple, Optional, Tuple
import urllib.request
from xml.etree import ElementTree


class Symbol(NamedTuple):
    name: str
    kind: str
    file: str
    line: int
    documented: bool


class Coverage(object):
    def __init__(self, root: str, src: str):
        self.root = root
        self.str = str
        self.symbols: Dict[str, Symbol] = {}

    @staticmethod
    def extract_name(node: ElementTree.Element) -> str:
        if node.find("./definition"):
            return node.find("./definition").text
        elif node.find("./name"):
            return node.find("./name").text
        elif node.find("./compoundname"):
            return node.find("./compoundname").text
        elif "id" in node.attrib:
            return node.attrib["id"]
        logging.warning("unable to determine name for node: {}".format(node))
        return ""

    @staticmethod
    def extract_kind(node: ElementTree.Element) -> str:
        kind = node.attrib.get("kind", "")
        if kind != "friend":
            return kind

        is_definition = (
            node.get("inline") == "yes" or node.find("initializer") is not None
        )
        if not is_definition:
            return kind

        friend_node = node.find("type")
        if friend_node is None:
            return kind
        friend_type = friend_node.text

        if friend_type == "friend class":
            return "class"
        elif friend_type == "friend struct":
            return "struct"
        elif friend_type == "friend union":
            return "union"
        else:
            return "function"

    @staticmethod
    def extract_docs(node: ElementTree.Element) -> bool:
        for type in ["briefdescription", "detaileddescription", "inbodydescription"]:
            desc = node.find("./{}".format(type))
            if desc is not None and "".join(desc.itertext()).strip():
                return True
        return False

    def extract_location(self, node: ElementTree.Element) -> Tuple[str, int]:
        loc = node.find("./location")
        if loc is None:
            return (self.src, 0)
        file = loc.attrib["file"]
        line = loc.attrib.get("line", 0)
        return (file, int(line))

    def process_symbol(self, xml: ElementTree.ElementTree) -> bool:
        name: str = self.extract_name(xml)
        kind: str = self.extract_kind(xml)
        docs: bool = self.extract_docs(xml)
        loc = self.extract_location(xml)
        logging.debug("Processed {} ({}): {}".format(name, kind, docs))
        self.symbols[xml.attrib["id"]] = Symbol(name, kind, loc[0], loc[1], docs)

        if kind == "enum":
            for val in xml.findall("./enumvalue"):
                name = self.extract_name(val)
                docs = self.extract_docs(val)
                self.symbols[val.attrib["id"]] = (
                    name,
                    "enumvalue",
                    loc[0],
                    loc[1],
                    docs,
                )
        return True

    def process_file(self, xml: ElementTree.ElementTree) -> bool:
        for node in [
            *xml.findall("./compounddef//memberdef"),
            *xml.findall("./compounddef"),
        ]:
            if not self.process_symbol(node):
                return False
        return True

    def process_index(self, xml: ElementTree.ElementTree) -> bool:
        logging.debug("Processing index")
        for entry in xml.findall("compound"):
            if "kind" not in entry.attrib or "refid" not in entry.attrib:
                logging.error(
                    "Invalid compound element, missing kind/refid: {}".format(
                        entry.attrib
                    )
                )
                return False
            elif entry.attrib["kind"] == "dir":
                continue

            file: str = path.join(self.root, entry.attrib["refid"]) + ".xml"
            try:
                fxml = ElementTree.parse(file)
            except Exception as e:
                logging.error("Failed to parse {}: {}".format(file, e))
                return False
            logging.debug("Processing {}".format(file))
            if not self.process_file(fxml):
                return False
        return True

    def process(self) -> bool:
        index: str = path.join(self.root, "index.xml")
        if not path.exists(index):
            logging.error("{} not found".format(index))
            return False
        try:
            xml = ElementTree.parse(index)
        except Exception as e:
            logging.error("Failed to parse {}: {}".format(index, e))
            return False

        if not self.process_index(xml):
            return False
        return True

    def filter(self, include: List[str], exclude: List[str]) -> bool:
        re_include = [re.compile(i) for i in include]
        re_exclude = [re.compile(e) for e in exclude]
        logging.debug("Filtering symbols")
        new_symbols: Dict[str, Symbol] = {}
        for key, value in self.symbols.items():
            if len(re_include) != 0 and not any(
                re.match(i, value.file) for i in re_include
            ):
                continue
            if any(re.match(e, value.file) for e in re_exclude):
                continue
            new_symbols[key] = value
        self.symbols = new_symbols
        return True


def text_format(coverage: Coverage, args: Namespace):
    covered = len([x for x in coverage.symbols.values() if x.documented])
    total = len(coverage.symbols.values())

    kinds = list(set([x.kind for x in coverage.symbols.values()]))

    if args.output == "-":
        file = sys.stdout
    else:
        file = open(args.output, "w")

    for kind in kinds:
        covered_kind = len(
            [x for x in coverage.symbols.values() if x.kind == kind and x.documented]
        )
        total_kind = len([x for x in coverage.symbols.values() if x.kind == kind])
        file.write(
            "{:>10}: {:6.1%} ({}/{})\n".format(
                kind.capitalize(), covered_kind / total_kind, covered_kind, total_kind
            )
        )
    file.write(
        "{:>10}: {:6.1%} ({}/{})\n".format("Total", covered / total, covered, total)
    )


def badge_format(coverage: Coverage, args: Namespace):
    covered = len([x for x in coverage.symbols.values() if x.documented])
    total = len(coverage.symbols.values())
    value = floor(covered / total * 100)

    if value <= 50:
        color = "red"
    elif value < 75:
        color = "yellow"
    elif value < 80:
        color = "yellowgreen"
    elif value < 90:
        color = "green"
    else:
        color = "brightgreen"

    url = "https://img.shields.io/badge/{}-{}%25-{}{}".format(
        args.label, value, color, args.badge_opts
    )

    req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
    badge = urllib.request.urlopen(req)

    if args.output == "-":
        file = sys.stdout
    else:
        file = open(args.output, "w")

    file.write(badge.read().decode("utf-8"))
    file.close()


def main():
    parser = ArgumentParser("doc-coverage.py")
    parser.add_argument(
        "-v", "--verbose", action="store_true", help="enable verbose output"
    )
    parser.add_argument(
        "-f",
        "--format",
        nargs="+",
        choices=["text", "badge"],
        default="text",
        help="output format",
    )
    parser.add_argument(
        "-e",
        "--exclude",
        type=str,
        nargs="*",
        default=[],
        help="list of regexes to exclude from coverage report",
    )
    parser.add_argument(
        "-i",
        "--include",
        type=str,
        nargs="*",
        default=[],
        help="list of regexes to include in the coverage report",
    )
    parser.add_argument(
        "--label",
        type=str,
        default="Doc%20Coverage",
        help="label to use when generating badge",
    )
    parser.add_argument(
        "--badge-opts",
        type=str,
        default="?style=flat-square",
        help="additional options to pass to shields.io badge",
    )
    parser.add_argument(
        "-o",
        "--output",
        type=str,
        default="-",
        help="filepath to write output to (- for stdout)",
    )
    parser.add_argument(
        "XML_DIR", type=str, help="path to the generated doxygen XML files"
    )
    parser.add_argument(
        "SOURCE_DIR", type=str, help="path to the root source directory of the project"
    )

    args = parser.parse_args()

    logging.basicConfig(
        format="%(asctime)s %(levelname)s: %(message)s",
        level=logging.DEBUG if args.verbose else logging.INFO,
    )

    coverage = Coverage(args.XML_DIR, args.SOURCE_DIR)
    if not coverage.process():
        return False
    if not coverage.filter(args.include, args.exclude):
        return False

    if "text" in args.format:
        text_format(coverage, args)
    if "badge" in args.format:
        badge_format(coverage, args)


if __name__ == "__main__":
    main()
