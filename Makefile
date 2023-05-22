all:
	$(MAKE) -C BSR 
	$(MAKE) -C BSR_APP 

debug:
	CFLAGS='-D _DEBUG' $(MAKE) -C BSR 
	CFLAGS='-D _DEBUG' $(MAKE) -C BSR_APP 

clean:
	$(MAKE) -C BSR clean
	$(MAKE) -C BSR_APP clean

.PHONY: all debug clean