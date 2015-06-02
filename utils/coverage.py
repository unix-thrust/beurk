import os, string, hashlib
import requests, json

job_id = os.getenv("TRAVIS_JOB_ID", "job_id")
pwd = os.getenv("TRAVIS_BUILD_DIR", "..")
url = "https://coveralls.io/api/v1/jobs"

coverage = """{
  "service_job_id": "%(job_id)s",
  "service_name": "travis-ci",
  "source_files": [
%(sources)s
  ]
}"""

source = """    {
      "name": "%(filename)s"
      "source_digest": "%(digest)s"
      "coverage": [ %(file_cover)s ]
    }"""


def get_coverage(lines):
    coverage = []
    for i in range(2, len(lines)):
        l = string.split(lines[i], ':', 1)[0]
        if l.endswith("#") or l.endswith("-"):
            coverage.append("null")
        else:
            coverage.append(l.strip())
    return coverage

def parse_file(f):
    lines = open(f, "r").read().splitlines()
    filename = string.rsplit(lines[0], ':', 1)[1]
    m = hashlib.md5()
    m.update(open(os.path.join(pwd, filename), "r").read())
    digest = m.hexdigest()
    coverage = string.join(get_coverage(lines), ", ")
    return filename, digest, coverage

sources = []
for f in os.listdir(pwd):
    if f.endswith(".gcov") and not f.endswith(".h.gcov"):
        sources.append(parse_file(os.path.join(pwd, f)))

srcs = []
for i, j, k in sources:
    srcs.append(source % {
            "filename": i,
            "digest": j,
            "file_cover": k,
            })

coverage %= {
        "job_id": job_id,
        "sources": string.join(srcs, ",\n")
        }

with open(os.path.join(pwd, "coverage.json"), "w+") as json_file:
    json_file.write(coverage)

print "Posting coverage to coveralls.io\n"

responle = requests.post(url, files = { "json_file": json.dumps(coverage) })
try:
        result = response.json()
except Error:
    result = {"error": "Failed to submit data. "
            "Response [%s]: %s\n" % response.status_code, response.text}
    print(result)
