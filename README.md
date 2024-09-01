# memo for check include dependency
import os
import re
import sys


def Extension(filename: str):
    pieces = filename.split(".")
    if len(pieces) != 2:
        return None
    extension = pieces[1]
    if extension == "cpp":
        return "cc"
    if extension == "cc":
        return "cc"
    if extension == "h":
        return "h"
    return None


def SearchIncludes(filename: str,
                   includeReference: list,
                   includeList: list = [],
                   systemHeaderList: list = [],
                   depth: int = 1):
    with open(filename, "r") as file:
        includes = re.findall("#include [<\"](.*)[\">]\n", file.read())
        for include in includes:
            in_ref = False
            for ref in includeReference:
                if include in ref:
                    include = ref
                    in_ref = True
                    break
            if not in_ref:
                systemHeaderList.append(include)
                continue
            if include in includeList:
                continue
            includeList.append(include)
            indent = ""
            for i in range(0, depth):
                indent = indent + "    "
            print(indent + include)
            try:
                SearchIncludes(include, includeReference, includeList,
                               systemIncludes, depth + 1)
            except OSError:
                print("Exception OSError opening " + filename +
                      " and searching " + include)
                raise OSError
    if depth == 1:
        print("    external dependency:")
        for i in systemHeaderList:
            print("        " + i)


Target = "./"
Headers = []
CC = []


for option in sys.argv:
    if option[0] == '-' and option[1] == 't':
        Target = option[2:]


for dirpath, dirnames, filenames in os.walk(Target):
    for fn in filenames:
        extension = Extension(fn)
        if extension is None:
            continue
        fn = os.path.join(dirpath, fn)
        if extension == "h":
            Headers.append(fn)
        elif extension == "cc":
            CC.append(fn)


for cc in CC:
    includes = []
    systemIncludes = []
    print(cc)
    SearchIncludes(cc, Headers, [])

"""
#include "common/MemoryIo.h"
#include "common/io.h"
#include <stdio.h>
#include <string.h>

class CoordinateWriter{
    protected:
        ~CoordinateWriter(){}
    public:
        virtual void Write(int x, int y, const char* content)=0;
};

CoordinateWriter* GetWriter(){
    class Impl : public CoordinateWriter{
        public:
        virtual void Write(int x, int y, const char* content){
            printf("x=%d, y=%d, %s\n", x, y, content);
        }
    };
    static Impl w;

    return &w;
}

struct Line{
    Line* next;
    const char* content;
    int x;
    int y;
};

class SortCoordinateWriter : public CoordinateWriter{
    private:
        void* getNewHead(){
            namespace n = common::io;
            int64_t new_offset;
            if(n::Seeker::eOk != w_.Seek(0, n::Seeker::Current, new_offset)){
                return 0;
            }

            return &(buffer_[new_offset]);
        }

        void addLine(Line* line){
            Line** indirect;
            for(indirect = &entry_;
                    *indirect != 0;
                    indirect = &((*indirect)->next)){
                if((*indirect)->y > line->y){
                    break;
                }

                if((*indirect)->y == line->y &&
                        (*indirect)->x > line->x){
                    break;
                }
            }

            line->next = *indirect;
            *indirect = line;
        }
    public:
        SortCoordinateWriter(char* buffer, int buffer_size) : 
            w_(buffer, buffer_size) ,
            buffer_(buffer),
            entry_(0){ }

        virtual void Write(int x, int y, const char* content){
            Line line;
            Line* node;

            line.content = static_cast<char*>(getNewHead());
            line.x = x;
            line.y = y;
            w_.Write(content, strlen(content));
            w_.Write("\0", 1);

            node = static_cast<Line*>(getNewHead());
            if(w_.Write(&line, sizeof(line)) != sizeof(line)){
                return;
            }
            addLine(node);
        }

        void Flush(){
            Line* indirect = entry_;
            while(indirect != 0){
                printf("%s", indirect->content);
                indirect = indirect->next;
            }
            printf("\n");
        }
    private:
        common::io::MemoryWriter w_;
        char* buffer_;
        Line* entry_;
};

int main(){
    CoordinateWriter* w = GetWriter();
    char buffer[1024];
    SortCoordinateWriter ww(buffer, sizeof(buffer));
    w = &ww;

    w->Write(0, 0, "Hello ");
    w->Write(30, 16, "Last");
    w->Write(30, 0, "World");
    w->Write(0, 16, "By S") ;
    w->Write(30, 0, "aaa");
    w->Write(45, 8, "XXX");

    ww.Flush();
    return 0;
}

"""
