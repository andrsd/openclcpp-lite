#!/usr/bin/env python3

import jinja2
import yaml
import argparse
import os
import sys
import importlib

parser = argparse.ArgumentParser(prog='gen.py', description='Generate file from a template')
parser.add_argument('--loc', '-l', dest='location', help='Location to search for templates')
parser.add_argument('--template', '-t', dest='templ', help='File with the template containing components')
parser.add_argument('--source', '-s', dest='src', help='File with the OpenCL code')
parser.add_argument('params', nargs='*', help='Parameters passed into the template.')

args = parser.parse_args()

def load_file(file_name):
    with open(file_name, "r") as f:
        lines = []
        while True:
            line = f.readline()
            if not line:
                break
            lines.append(line.rstrip('\n'))
        f.close()
        return lines


# parameters that will be passed into the template
templ_params = {}

# process parameters passed via command line
for str_param in vars(args)['params']:
    # FIXME: do a regexp here instead of a split
    (name, value) = str_param.split('=')
    templ_params[name] = value

if args.location is None:
    args.location, args.templ = os.path.split(args.templ)

if args.location and args.templ:
    tmpl_loader = jinja2.FileSystemLoader(searchpath=args.location)
    tmpl_env = jinja2.Environment(loader=tmpl_loader)

    if args.src:
        templ_params["src"] = load_file(args.src)

    # render the full input file
    templ = tmpl_env.get_template(args.templ)
    out = templ.render(
        templ_params,
    )

    print(out)
