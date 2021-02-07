SUBDIRS = ForwarDS ForwarDSTemplate

all:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

clean:
	@for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir; done

ForwarDS:
	@$(MAKE) -C ForwarDS

ForwarDSTemplate:
	@$(MAKE) -C ForwarDSTemplate

.PHONY: $(SUBDIRS) clean