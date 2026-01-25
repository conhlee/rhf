import os
import sys
import argparse
import io
import struct
from typing import Self
# import time

class Yaz0Object:
    def __init__(self, file: io.BufferedReader, filename: str):
        self.filename = filename

        file.seek(0, os.SEEK_END)
        fileSize = file.tell()
        file.seek(0, os.SEEK_SET)

        identifier = file.read(4)
        if identifier != b"Yaz0":
            print(f"[RCAD LABEL] Error decompressing compressed resource {self.filename}: identifier is nonmatch", file=sys.stderr)
            sys.exit(1)

        self.decompressedSize = struct.unpack(">I", file.read(4))[0]
        file.seek(8, os.SEEK_CUR) # Skip reserved fields ..

        self.compressedData = file.read(fileSize - file.tell())

    def decompress(self) -> bytes:
        dstBuffer = bytearray(self.decompressedSize)

        opByte = 0
        opMask = 0

        srcPos = 0
        dstPos = 0

        while dstPos < self.decompressedSize:
            if opMask == 0: # No more operation bits left; refresh
                opByte = self.compressedData[srcPos]
                srcPos += 1

                opMask = (1 << 7)
            
            if (opByte & opMask) != 0: # Copy one byte.
                dstBuffer[dstPos] = self.compressedData[srcPos]
                srcPos += 1
                dstPos += 1
            else: # Run-length data.
                distToDest = (self.compressedData[srcPos] << 8) | self.compressedData[srcPos + 1]
                srcPos += 2

                runSrcIdx = dstPos - (distToDest & 0xFFF)

                # runLen = 0
                if (distToDest >> 12) == 0:
                    runLen = self.compressedData[srcPos] + 0x12
                    srcPos += 1
                else:
                    runLen = (distToDest >> 12) + 2

                while runLen > 0:
                    dstBuffer[dstPos] = dstBuffer[runSrcIdx - 1]
                    runLen -= 1
                    dstPos += 1
                    runSrcIdx += 1
            
            opMask >>= 1

        return bytes(dstBuffer)

class DARCHNode:
    def __init__(self):
        self.name = ""
        self.children = None
        self.data = None

    def isInitialized(self):
        if (self.children == None) and (self.data == None):
            return False
        return True

    def initDirectory(self, name: str):
        self.name = name
        self.children: list[DARCHNode] = []
        self.data = None
    def initFile(self, name: str):
        self.name = name
        self.children = None
        self.data = bytes()

    def isDirectory(self) -> bool:
        if (self.data != None):
            return False
        return True
    
    def findChild(self, childName) -> Self | None:
        if not self.isDirectory():
            print(f"[RCAD LABEL] Error: programmer messed up !! (findChild called on file node)", file=sys.stderr)
            sys.exit(1)

        for child in self.children:
            if child.name == childName:
                return child
            
        return None

class DARCHObject:
    def __init__(self, file: io.BufferedReader, filename: str):
        self.filename = filename

        # file.seek(0, os.SEEK_END)
        # fileSize = file.tell()
        # file.seek(0, os.SEEK_SET)

        identifier = file.read(4)
        if identifier != b"U\xAA8-":
            print(f"[RCAD LABEL] Error parsing archive {self.filename}: identifier is nonmatch", file=sys.stderr)
            sys.exit(1)

        self.nodeSectionStart = struct.unpack(">I", file.read(4))[0]
        self.nodeSectionSize = struct.unpack(">I", file.read(4))[0]
        self.dataSectionStart = struct.unpack(">I", file.read(4))[0]

        file.seek(self.nodeSectionStart)

        # Read root node ..
        file.seek(8, os.SEEK_CUR) # skip isDirAndNameOffset, parent
        nodeCount = struct.unpack(">I", file.read(4))[0] # read nextOutOfDir

        SIZEOF_BIN_NODE = 12
        stringPoolOffset = self.nodeSectionStart + (SIZEOF_BIN_NODE * nodeCount)

        self.structure = DARCHNode ()
        self.structure.initDirectory("")

        dirStack = [] # [ DARCHNode (ref), nextOutOfDir ]
        currentDirectory = self.structure

        dirStack.append([ currentDirectory, 0 ])

        i = 1 # Skip root node ..
        while i < nodeCount:
            dIsDirAndNameOffset = struct.unpack(">I", file.read(4))[0]

            dIsDir = ((dIsDirAndNameOffset & 0xFF000000) >> 24) != 0
            dNameOffset = dIsDirAndNameOffset & 0xFFFFFF

            initialPos = file.tell()

            file.seek(stringPoolOffset + dNameOffset)

            nodeNameBytes = bytearray()
            while True:
                byte = file.read(1)
                if not byte or byte == b'\x00':
                    break
                nodeNameBytes.extend(byte)
            nodeName = nodeNameBytes.decode("ascii")

            file.seek(initialPos) # Restore

            newNode = DARCHNode()

            if dIsDir: # Directory ..
                # dParent = struct.unpack(">I", file.read(4))[0]
                file.seek(4, os.SEEK_CUR) # We don't need the parent field, so just skip over

                dNextOutOfDir = struct.unpack(">I", file.read(4))[0]

                newNode.initDirectory(nodeName)

                currentDirectory.children.append(newNode)

                currentDirectory = newNode
                dirStack.append([ currentDirectory, dNextOutOfDir ])
            else: # File ..
                dDataOffset = struct.unpack(">I", file.read(4))[0]
                dDataSize = struct.unpack(">I", file.read(4))[0]

                newNode.initFile(nodeName)

                initialPos = file.tell()

                file.seek(dDataOffset)
                newNode.data = file.read(dDataSize)

                file.seek(initialPos) # Restore

                currentDirectory.children.append(newNode)
            
            while (len(dirStack) != 0) and (dirStack[-1][1] == i + 1):
                dirStack.pop()
                if len(dirStack) != 0:
                    currentDirectory = dirStack[-1][0]
            
            i += 1

scriptDir = os.path.dirname(os.path.realpath(__file__))
rootDir = os.path.abspath(os.path.join(scriptDir, ".."))

def getOutputPath(szsPath, labelFileName, includeRoot="include"):
    # find the directory path after "cellanim/"
    components = os.path.normpath(szsPath).split(os.sep)
    if "cellanim" not in components:
        print(f"[RCAD LABEL] 'cellanim' not found in path: {szsPath}", file=sys.stderr)
        sys.exit(1)

    idx = components.index("cellanim") + 1
    relSubdir = os.path.join(*components[idx:]) # e.g. cellanim/agbClap/ver0/cellanim.szs
    relDir = os.path.dirname(relSubdir) # strip "cellanim.szs"
    return os.path.join(includeRoot, relDir, labelFileName)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("szsFiles", nargs="+", help="paths to cellanim archives")
    parser.add_argument("--outputDir", "-o", help="path to output directory")
    args = parser.parse_args()

    for idx, szsPath in enumerate(args.szsFiles, start=1):
        print(f"[RCAD LABEL] Processing ({idx}/{len(args.szsFiles)}): {szsPath}", flush=True)

        if not os.path.isfile(szsPath):
            print(f"\n[RCAD LABEL] SZS does not exist: {szsPath}, reconfigure?", file=sys.stderr)
            continue

        with open(szsPath, "rb") as szsFile:
            szsObject = Yaz0Object(szsFile, szsPath)

            darchFile = io.BytesIO(szsObject.decompress())
            darchObject = DARCHObject(io.BufferedReader(darchFile), szsPath)

            rootDir = darchObject.structure.findChild(".")
            if rootDir == None:
                print(f"\n[RCAD LABEL] SZS at {szsPath} does not contain the root directory \".\" ..", file=sys.stderr)
                continue

            labelFiles: list[DARCHNode] = []

            for node in rootDir.children:
                if not node.isDirectory():
                    if node.name.startswith("rcad_") and node.name.endswith("_labels.h"):
                        labelFiles.append(node)
                
            if len(labelFiles) == 0:
                print(f"\n[RCAD LABEL] No rcad_*_labels.h found in {szsPath} ..", file=sys.stderr)
                continue
            
            for labelFile in labelFiles:
                outputPath = getOutputPath(szsPath, labelFile.name, args.outputDir)
                os.makedirs(os.path.dirname(outputPath), exist_ok=True)

                asUTF8 = labelFile.data.decode("shift_jis")
                with open(outputPath, 'w', encoding='utf-8') as f:
                    f.write(asUTF8)

    print("\n[RCAD LABEL] All done!")

if __name__ == "__main__":
    main()
