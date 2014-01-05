# subdirectories
SUBDIRS = libvmdconv src

# chain with ChangeLog
dist-hook: ChangeLog

# work when configure.ac is updated
ChangeLog: configure.ac
	git log --stat --name-only --date=short --abbrev-commit > ChangeLog