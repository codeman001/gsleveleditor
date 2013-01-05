.SUFFIXES : .pvr .tga

PVR_TOOLS = ../../tools/pvrCompress/PVRTexTool.exe

# sources

SRC_TGA := $(shell /usr/bin/find ./level -path "*.svn" -prune -o -name "*.tga")
SRC_TGA := $(SRC_TGA) $(shell /usr/bin/find ./mesh -path "*.svn" -prune -o -name "*.tga")
SRC_TGA := $(SRC_TGA) $(shell /usr/bin/find ./particle -path "*.svn" -prune -o -name "*.tga")
SRC_TGA := $(SRC_TGA) $(shell /usr/bin/find ./texture -path "*.svn" -prune -o -name "*.tga")
DST_TGA2PVR := $(patsubst %.tga,%.pvr,$(SRC_TGA))

# rules

.tga.pvr : ; $(PVR_TOOLS) -pvrtcbest -fPVRTC4 -i ./$< -o ./$@


# targets

all : image_pvr

image_pvr : $(DST_TGA2PVR)

CLEAN_IMAGE_PVR = $(shell /usr/bin/find . -name "*.pvr")
clean_image_pvr : ; rm -f $(CLEAN_IMAGE_PVR)

clean: clean_imagePVR