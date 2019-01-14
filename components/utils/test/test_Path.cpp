/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <limits.h>
#include <gtest/gtest.h>

#include <utils/Path.h>

#include <iosfwd>
#include <string>
#include <vector>

#ifndef PATH_MAX    // should be in <limits.h>
#define PATH_MAX    4096
#endif

using namespace utils;

void mk_file(Path &p, const char *str) {
    FILE *f = ::fopen(p.c_str(), "w");
    ::fprintf(f, "placeholder %s", str);
    ::fclose(f);
};

#ifdef WINDOWS

TEST(PathTest, Constructor) {
    // Ensure the path is canonical
    Path path("\\out\\blue\\..\\bin\\.\\test_path");
    EXPECT_EQ(Path::canonical("\\out\\blue\\..\\bin\\.\\test_path"), path.get());
}

TEST(PathTest, Conversion) {
    Path r("\\out\\blue\\bin");
    std::string s(r);
    EXPECT_EQ(r, s);
}

TEST(PathTest, Assignment) {
    Path r("\\out\\blue\\bin");

    r = "\\out\\blue";
    EXPECT_EQ("\\out\\blue", r.get());

    std::string s("\\etc");
    r = s;
    EXPECT_EQ("\\etc", r.get());

    r = Path("\\bin");
    EXPECT_EQ("\\bin", r.get());

    r.set("\\blue\\bin");
    EXPECT_EQ("\\blue\\bin", r.get());
}

TEST(PathTest, Concatenate) {
    Path p0, p1("name.png");
    EXPECT_EQ("name.png", (p0 + p1).get());

    Path root("\\Volumes\\Replicant\\blue");

    Path r;
    r = root.concat("");
    EXPECT_EQ("\\Volumes\\Replicant\\blue", r.get());

    r = root.concat("\\out\\bin");
    EXPECT_EQ("\\out\\bin", r.get());

    r = root.concat("out\\bin");
    EXPECT_EQ("\\Volumes\\Replicant\\blue\\out\\bin", r.get());

    r = root.concat(".");
    EXPECT_EQ("\\Volumes\\Replicant\\blue", r.get());

    r = root.concat("..");
    EXPECT_EQ("\\Volumes\\Replicant", r.get());

    r = root.concat("\\");
    EXPECT_EQ("\\", r.get());

    r = root.concat("..\\remote-blue");
    EXPECT_EQ("\\Volumes\\Replicant\\remote-blue", r.get());

    r = root.concat("..\\remote-blue");
    EXPECT_EQ(r, root + Path("..\\remote-blue"));
    EXPECT_EQ(r, root + "..\\remote-blue");

    r = "\\out\\bin";
    r.concat_to_self("..\\bin");
    EXPECT_EQ("\\out\\bin", r.get());

    r += ".\\resources";
    EXPECT_EQ("\\out\\bin\\resources", r.get());
}

TEST(PathTest, Sanitization) {
    std::string r;

    // An empty path remains empty
    r = Path::canonical("");
    EXPECT_EQ("", r);

    // A single \\ is preserved
    r = Path::canonical("\\");
    EXPECT_EQ("\\", r);

    // A single \\ is preserved
    r = Path::canonical("//");
    EXPECT_EQ("\\", r);

    // A leading . is preserved
    r = Path::canonical(".\\out");
    EXPECT_EQ(".\\out", r);

    // A leading . is preserved
    r = Path::canonical(".");
    EXPECT_EQ(".", r);

    // A leading .. is preserved
    r = Path::canonical("\\.");
    EXPECT_EQ("\\", r);

    // A leading .. is preserved
    r = Path::canonical("..\\out");
    EXPECT_EQ("..\\out", r);

    // A leading .. is preserved
    r = Path::canonical("\\..");
    EXPECT_EQ("\\", r);

    // A leading \\ is preserved
    r = Path::canonical("\\out");
    EXPECT_EQ("\\out", r);

    // A middle . is removed
    r = Path::canonical("out\\.\\bin");
    EXPECT_EQ("out\\bin", r);

    // two middle . are removed
    r = Path::canonical("out\\.\\.\\bin");
    EXPECT_EQ("out\\bin", r);

    // three middle . are removed
    r = Path::canonical("out\\.\\.\\.\\bin");
    EXPECT_EQ("out\\bin", r);

    // a starting . is kept
    r = Path::canonical(".\\bin");
    EXPECT_EQ(".\\bin", r);

    // several starting . are collapsed to one
    r = Path::canonical(".\\.\\bin");
    EXPECT_EQ(".\\bin", r);

    // several starting . are collapsed to one
    r = Path::canonical(".\\.\\.\\bin");
    EXPECT_EQ(".\\bin", r);

    // A middle .. is removed and the previous segment is removed
    r = Path::canonical("out\\blue\\..\\bin");
    EXPECT_EQ("out\\bin", r);

    // Special case of the previous test
    // A .. in second spot pops to an empty stack
    r = Path::canonical("out\\..\\bin");
    EXPECT_EQ("bin", r);

    // Special case of the previous test
    // A .. in second spot pops to an empty stack
    r = Path::canonical("out\\..\\..\\bin");
    EXPECT_EQ("..\\bin", r);

    // make sure it works with several ..\\ 
    r = Path::canonical("..\\..\\bin");
    EXPECT_EQ("..\\..\\bin", r);

    // make sure to test odd\\even numbers of ..\\ and more than one
    r = Path::canonical("..\\..\\..\\bin");
    EXPECT_EQ("..\\..\\..\\bin", r);

    // check odd and more than 1 or 2 ..\\ in the middle
    r = Path::canonical("out\\..\\..\\..\\bin");
    EXPECT_EQ("..\\..\\bin", r);

    // Two or more slashes is the same as one
    r = Path::canonical("out\\blue//bin");
    EXPECT_EQ("out\\blue\\bin", r);

    // A trailing \\ is preserved
    r = Path::canonical("out\\blue\\bin\\");
    EXPECT_EQ("out\\blue\\bin\\", r);

    // Both leading and trailing \\ are preserved
    r = Path::canonical("\\out\\blue\\bin\\");
    EXPECT_EQ("\\out\\blue\\bin\\", r);

    // preserve a segment starting with a .
    r = Path::canonical("\\out\\.blue\\bin\\");
    EXPECT_EQ("\\out\\.blue\\bin\\", r);

    // remove a \\.\\ following a ..
    r = Path::canonical("\\out\\..\\.\\bin\\");
    EXPECT_EQ("\\bin\\", r);

    // remove a \\.\\ following a ..
    r = Path::canonical("..\\.\\bin\\");
    EXPECT_EQ("..\\bin\\", r);

    // collapse multiple \\ 
    r = Path::canonical("////");
    EXPECT_EQ("\\", r);

    // collapse multiple \\ 
    r = Path::canonical("\\aaa//\\bbb\\");
    EXPECT_EQ("\\aaa\\bbb\\", r);

    // collapse multiple \\ 
    r = Path::canonical("//\\.//\\");
    EXPECT_EQ("\\", r);

    // multiple ..
    r = Path::canonical("..\\out\\..\\in");
    EXPECT_EQ("..\\in", r);

    // \\..
    r = Path::canonical("\\..\\out\\..\\in");
    EXPECT_EQ("\\in", r);

    // No sanitizing required
    r = Path::canonical("out");
    EXPECT_EQ("out", r);
}

TEST(PathTest, GetParent) {
    std::string r;
    Path p("\\out\\bin");
    r = p.parent();
    EXPECT_EQ("\\out\\", r);

    p = "\\out\\bin\\";
    r = p.parent();
    EXPECT_EQ("\\out\\", r);

    p = "out\\bin";
    r = p.parent();
    EXPECT_EQ("out\\", r);

    p = "out\\bin\\";
    r = p.parent();
    EXPECT_EQ("out\\", r);

    p = "out";
    r = p.parent();
    EXPECT_EQ("", r);

    p = "\\out";
    r = p.parent();
    EXPECT_EQ("\\", r);

    p = "";
    r = p.parent();
    EXPECT_EQ("", r);

    p = "\\";
    r = p.parent();
    EXPECT_EQ("\\", r);
}

TEST(PathTest, GetName) {
    Path p("\\out\\bin");
    EXPECT_EQ("bin", p.name());

    p = "\\out\\bin\\";
    EXPECT_EQ("bin", p.name());

    p = "\\";
    EXPECT_EQ("\\", p.name());

    p = "out";
    EXPECT_EQ("out", p.name());
}

TEST(PathTest, Exists) {
    EXPECT_FALSE(Path("").exists());
    EXPECT_TRUE(Path("\\").exists());
    EXPECT_FALSE(Path("this\\better\\not\\be\\a\\path").exists());
}

TEST(PathTest, Split) {
    std::vector<std::string> segments;

    segments = Path("").split();
    EXPECT_EQ(0, segments.size());

    segments = Path("\\").split();
    EXPECT_EQ(1, segments.size());
    EXPECT_EQ("\\", segments[0]);

    segments = Path("out\\blue\\bin").split();
    EXPECT_EQ(3, segments.size());
    EXPECT_EQ("out", segments[0]);
    EXPECT_EQ("blue", segments[1]);
    EXPECT_EQ("bin", segments[2]);

    segments = Path("\\out\\blue\\bin").split();
    EXPECT_EQ(4, segments.size());
    EXPECT_EQ("\\", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("blue", segments[2]);
    EXPECT_EQ("bin", segments[3]);

    segments = Path("\\out\\blue\\bin\\").split();
    EXPECT_EQ(4, segments.size());
    EXPECT_EQ("\\", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("blue", segments[2]);
    EXPECT_EQ("bin", segments[3]);
}

TEST(PathTest, CurrentDirectory) {
    Path p(Path::pwd());
    EXPECT_FALSE(p.is_empty());
    EXPECT_TRUE(p.is_absolute());
    EXPECT_TRUE(p.exists());
    EXPECT_TRUE(p.is_directory());
    EXPECT_FALSE(p.is_file());
}

TEST(PathTest, CurrentExecutable) {
    Path p(Path::current_executable());
    EXPECT_FALSE(p.is_empty());
    EXPECT_TRUE(p.is_absolute());
    EXPECT_TRUE(p.exists());
    EXPECT_FALSE(p.is_directory());
    EXPECT_TRUE(p.is_file());
}

TEST(PathTest, AbsolutePath) {
    Path cwd = Path::pwd();

    Path p;
    p = Path("\\out\\blue\\bin");
    EXPECT_TRUE(p.is_absolute());

    p = p.absolute_path();
    EXPECT_EQ("\\out\\blue\\bin", p.get());

    p = Path("..\\bin").absolute_path();
    EXPECT_NE(cwd, p);
    EXPECT_TRUE(p.is_absolute());
}

TEST(PathTest, IsFile) {
    Path dir(Path::pwd());
    Path exe(Path::current_executable());
    EXPECT_TRUE(exe.is_file());
    EXPECT_FALSE(dir.is_file());
}

TEST(PathTest, IsDirectory) {
    Path dir(Path::pwd());
    Path exe(Path::current_executable());
    EXPECT_FALSE(exe.is_directory());
    EXPECT_TRUE(dir.is_directory());
}

TEST(PathTest, GetExtension) {
    Path p("\\out\\bin\\somefile.txt");
    EXPECT_EQ(p.extension(), "txt");

    p = Path("\\out\\bin\\somefilewithoutextension");
    EXPECT_EQ(p.extension(), "");

    p = Path("\\out\\bin\\.tempdir\\somefile.txt.bak");
    EXPECT_EQ(p.extension(), "bak");

    p = Path("\\out\\bin\\.tempfile");
    EXPECT_EQ(p.extension(), "");

    p = Path("\\out\\bin\\endsindot.");
    EXPECT_EQ(p.extension(), "");

    p = Path::pwd();
    EXPECT_EQ(p.extension(), "");

    p = Path();
    EXPECT_EQ(p.extension(), "");
}

TEST(WinPathTest, Sanitization) {
    std::string r;

    // An empty path remains empty
    r = Path::canonical("");
    EXPECT_EQ("", r);

    // A single / is preserved
    r = Path::canonical("\\");
    EXPECT_EQ("\\", r);

    // Unix style paths are converted to Windows style
    r = Path::canonical("out/./././bin/foo/../../bar");
    EXPECT_EQ("out\\bar", r);

    // A mix of Unix style paths and Windows style
    r = Path::canonical("out/.\\././bin/foo\\../..\\bar");
    EXPECT_EQ("out\\bar", r);

    // Disk designation
    r = Path::canonical("C:\\");
    EXPECT_EQ("C:\\", r);

    // Disk designation
    r = Path::canonical("C:\\out\\bin");
    EXPECT_EQ("C:\\out\\bin", r);

    // Collapse .. with disk designation
    r = Path::canonical("C:\\out\\bin\\..\\foo");
    EXPECT_EQ("C:\\out\\foo", r);

    // Collapse multiple .. with disk designation
    r = Path::canonical("C:\\out\\bin\\..\\..\\foo");
    EXPECT_EQ("C:\\foo", r);

    // Collapse . with disk designation
    r = Path::canonical("C:\\out\\.\\foo");
    EXPECT_EQ("C:\\out\\foo", r);

    // Collapse multiple . with disk designation
    r = Path::canonical("C:\\out\\.\\.\\foo");
    EXPECT_EQ("C:\\out\\foo", r);

    // Collapse multiple . and .. with disk designation
    r = Path::canonical("C:\\out\\bin\\.\\..\\..\\foo");
    EXPECT_EQ("C:\\foo", r);

    // make sure it works with several ../
    r = Path::canonical("..\\..\\bin");
    EXPECT_EQ("..\\..\\bin", r);
}

TEST(WinPathTest, AbsolutePath) {
    Path cwd = Path::pwd();

    Path p;
    p = Path("C:\\out\\blue\\bin");
    EXPECT_TRUE(p.is_absolute());

    p = p.absolute_path();
    EXPECT_EQ("C:\\out\\blue\\bin", p.get());

    p = Path("../bin").absolute_path();
    EXPECT_NE(cwd, p);
    EXPECT_TRUE(p.is_absolute());
}

TEST(WinPathTest, Split) {
    std::vector<std::string> segments;

    segments = Path("").split();
    EXPECT_EQ(0, segments.size());

    segments = Path("\\").split();
    EXPECT_EQ(1, segments.size());
    EXPECT_EQ("\\", segments[0]);

    segments = Path("C:\\").split();
    EXPECT_EQ(1, segments.size());
    EXPECT_EQ("C:\\", segments[0]);

    segments = Path("\\out\\blue\\bin").split();
    EXPECT_EQ(4, segments.size());
    EXPECT_EQ("\\", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("blue", segments[2]);
    EXPECT_EQ("bin", segments[3]);

    segments = Path("/out\\foo/blue\\bin/").split();
    EXPECT_EQ(5, segments.size());
    EXPECT_EQ("\\", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("foo", segments[2]);
    EXPECT_EQ("blue", segments[3]);
    EXPECT_EQ("bin", segments[4]);

    segments = Path("C:\\out\\foo/blue\\bin/").split();
    EXPECT_EQ(5, segments.size());
    EXPECT_EQ("C:\\", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("foo", segments[2]);
    EXPECT_EQ("blue", segments[3]);
    EXPECT_EQ("bin", segments[4]);
}

TEST(WinPathTest, Concatenate) {
    Path root("C:\\Volumes\\Replicant\\blue");

    Path r;
    r = root.concat("");
    EXPECT_EQ("C:\\Volumes\\Replicant\\blue", r.get());

    r = root.concat("C:\\out\\bin");
    EXPECT_EQ("C:\\out\\bin", r.get());

    r = root.concat("out\\bin");
    EXPECT_EQ("C:\\Volumes\\Replicant\\blue\\out\\bin", r.get());

    r = root.concat(".");
    EXPECT_EQ("C:\\Volumes\\Replicant\\blue", r.get());

    r = root.concat("..");
    EXPECT_EQ("C:\\Volumes\\Replicant", r.get());

    r = root.concat("C:\\");
    EXPECT_EQ("C:\\", r.get());

    r = root.concat("..\\remote-blue");
    EXPECT_EQ("C:\\Volumes\\Replicant\\remote-blue", r.get());

    r = root.concat("..\\remote-blue");
    EXPECT_EQ(r, root + Path("../remote-blue"));
    EXPECT_EQ(r, root + "../remote-blue");

    r = "C:\\out\\bin";
    r.concat_to_self("../bin");
    EXPECT_EQ("C:\\out\\bin", r.get());

    r += "./resources";
    EXPECT_EQ("C:\\out\\bin\\resources", r.get());

    // Unix-style separators work too
    r = root.concat("out/bin/foo/bar");
    EXPECT_EQ("C:\\Volumes\\Replicant\\blue\\out\\bin\\foo\\bar", r.get());
}

TEST(WinPathTest, GetParent) {
    std::string r;
    Path p("C:\\out\\bin");
    r = p.parent();
    EXPECT_EQ("C:\\out\\", r);

    p = "C:\\out\\bin\\";
    r = p.parent();
    EXPECT_EQ("C:\\out\\", r);

    p = "out\\bin";
    r = p.parent();
    EXPECT_EQ("out\\", r);

    p = "out\\bin\\";
    r = p.parent();
    EXPECT_EQ("out\\", r);

    p = "out";
    r = p.parent();
    EXPECT_EQ("", r);

    p = "C:\\out";
    r = p.parent();
    EXPECT_EQ("C:\\", r);

    p = "";
    r = p.parent();
    EXPECT_EQ("", r);

    p = "C:\\";
    r = p.parent();
    EXPECT_EQ("C:\\", r);
}

#else

TEST(PathTest, Constructor) {
    // Ensure the path is canonical
    Path path("/out/blue/../bin/./test_path");
    EXPECT_EQ(Path::canonical("/out/blue/../bin/./test_path"), path.get());
}

TEST(PathTest, Conversion) {
    Path r("/out/blue/bin");
    std::string s(r);
    EXPECT_EQ(r, s);
}

TEST(PathTest, Assignment) {
    Path r("/out/blue/bin");

    r = "/out/blue";
    EXPECT_EQ("/out/blue", r.get());

    std::string s("/etc");
    r = s;
    EXPECT_EQ("/etc", r.get());

    r = Path("/bin");
    EXPECT_EQ("/bin", r.get());

    r.set("/blue/bin");
    EXPECT_EQ("/blue/bin", r.get());
}

TEST(PathTest, Concatenate) {
    Path root("/Volumes/Replicant/blue");

    Path r;
    r = root.concat("");
    EXPECT_EQ("/Volumes/Replicant/blue", r.get());

    r = root.concat("/out/bin");
    EXPECT_EQ("/out/bin", r.get());

    r = root.concat("out/bin");
    EXPECT_EQ("/Volumes/Replicant/blue/out/bin", r.get());

    r = root.concat(".");
    EXPECT_EQ("/Volumes/Replicant/blue", r.get());

    r = root.concat("..");
    EXPECT_EQ("/Volumes/Replicant", r.get());

    r = root.concat("/");
    EXPECT_EQ("/", r.get());

    r = root.concat("../remote-blue");
    EXPECT_EQ("/Volumes/Replicant/remote-blue", r.get());

    r = root.concat("../remote-blue");
    EXPECT_EQ(r, root + Path("../remote-blue"));
    EXPECT_EQ(r, root + "../remote-blue");

    r = "/out/bin";
    r.concat_to_self("../bin");
    EXPECT_EQ("/out/bin", r.get());

    r += "./resources";
    EXPECT_EQ("/out/bin/resources", r.get());
}

TEST(PathTest, Sanitization) {
    std::string r;

    // An empty path remains empty
    r = Path::canonical("");
    EXPECT_EQ("", r);

    // A single / is preserved
    r = Path::canonical("/");
    EXPECT_EQ("/", r);

    // A single / is preserved
    r = Path::canonical("//");
    EXPECT_EQ("/", r);

    // A leading . is preserved
    r = Path::canonical("./out");
    EXPECT_EQ("./out", r);

    // A leading . is preserved
    r = Path::canonical(".");
    EXPECT_EQ(".", r);

    // A leading .. is preserved
    r = Path::canonical("/.");
    EXPECT_EQ("/", r);

    // A leading .. is preserved
    r = Path::canonical("../out");
    EXPECT_EQ("../out", r);

    // A leading .. is preserved
    r = Path::canonical("/..");
    EXPECT_EQ("/", r);

    // A leading / is preserved
    r = Path::canonical("/out");
    EXPECT_EQ("/out", r);

    // A middle . is removed
    r = Path::canonical("out/./bin");
    EXPECT_EQ("out/bin", r);

    // two middle . are removed
    r = Path::canonical("out/././bin");
    EXPECT_EQ("out/bin", r);

    // three middle . are removed
    r = Path::canonical("out/./././bin");
    EXPECT_EQ("out/bin", r);

    // a starting . is kept
    r = Path::canonical("./bin");
    EXPECT_EQ("./bin", r);

    // several starting . are collapsed to one
    r = Path::canonical("././bin");
    EXPECT_EQ("./bin", r);

    // several starting . are collapsed to one
    r = Path::canonical("./././bin");
    EXPECT_EQ("./bin", r);

    // A middle .. is removed and the previous segment is removed
    r = Path::canonical("out/blue/../bin");
    EXPECT_EQ("out/bin", r);

    // Special case of the previous test
    // A .. in second spot pops to an empty stack
    r = Path::canonical("out/../bin");
    EXPECT_EQ("bin", r);

    // Special case of the previous test
    // A .. in second spot pops to an empty stack
    r = Path::canonical("out/../../bin");
    EXPECT_EQ("../bin", r);

    // make sure it works with several ../
    r = Path::canonical("../../bin");
    EXPECT_EQ("../../bin", r);

    // make sure to test odd/even numbers of ../ and more than one
    r = Path::canonical("../../../bin");
    EXPECT_EQ("../../../bin", r);

    // check odd and more than 1 or 2 ../ in the middle
    r = Path::canonical("out/../../../bin");
    EXPECT_EQ("../../bin", r);

    // Two or more slashes is the same as one
    r = Path::canonical("out/blue//bin");
    EXPECT_EQ("out/blue/bin", r);

    // A trailing / is preserved
    r = Path::canonical("out/blue/bin/");
    EXPECT_EQ("out/blue/bin/", r);

    // Both leading and trailing / are preserved
    r = Path::canonical("/out/blue/bin/");
    EXPECT_EQ("/out/blue/bin/", r);

    // preserve a segment starting with a .
    r = Path::canonical("/out/.blue/bin/");
    EXPECT_EQ("/out/.blue/bin/", r);

    // remove a /./ following a ..
    r = Path::canonical("/out/.././bin/");
    EXPECT_EQ("/bin/", r);

    // remove a /./ following a ..
    r = Path::canonical(".././bin/");
    EXPECT_EQ("../bin/", r);

    // collapse multiple /
    r = Path::canonical("////");
    EXPECT_EQ("/", r);

    // collapse multiple /
    r = Path::canonical("/aaa///bbb/");
    EXPECT_EQ("/aaa/bbb/", r);

    // collapse multiple /
    r = Path::canonical("///.///");
    EXPECT_EQ("/", r);

    // multiple ..
    r = Path::canonical("../out/../in");
    EXPECT_EQ("../in", r);

    // /..
    r = Path::canonical("/../out/../in");
    EXPECT_EQ("/in", r);

    // No sanitizing required
    r = Path::canonical("out");
    EXPECT_EQ("out", r);
}

TEST(PathTest, GetParent) {
    std::string r;
    Path p("/out/bin");
    r = p.parent();
    EXPECT_EQ("/out/", r);

    p = "/out/bin/";
    r = p.parent();
    EXPECT_EQ("/out/", r);

    p = "out/bin";
    r = p.parent();
    EXPECT_EQ("out/", r);

    p = "out/bin/";
    r = p.parent();
    EXPECT_EQ("out/", r);

    p = "out";
    r = p.parent();
    EXPECT_EQ("", r);

    p = "/out";
    r = p.parent();
    EXPECT_EQ("/", r);

    p = "";
    r = p.parent();
    EXPECT_EQ("", r);

    p = "/";
    r = p.parent();
    EXPECT_EQ("/", r);
}

TEST(PathTest, GetName) {
    Path p("/out/bin");
    EXPECT_EQ("bin", p.name());

    p = "/out/bin/";
    EXPECT_EQ("bin", p.name());

    p = "/";
    EXPECT_EQ("/", p.name());

    p = "out";
    EXPECT_EQ("out", p.name());
}

TEST(PathTest, Exists) {
    EXPECT_FALSE(Path("").exists());
    EXPECT_TRUE(Path("/").exists());
    EXPECT_FALSE(Path("this/better/not/be/a/path").exists());
}

TEST(PathTest, Split) {
    std::vector<std::string> segments;

    segments = Path("").split();
    EXPECT_EQ(0, segments.size());

    segments = Path("/").split();
    EXPECT_EQ(1, segments.size());
    EXPECT_EQ("/", segments[0]);

    segments = Path("out/blue/bin").split();
    EXPECT_EQ(3, segments.size());
    EXPECT_EQ("out", segments[0]);
    EXPECT_EQ("blue", segments[1]);
    EXPECT_EQ("bin", segments[2]);

    segments = Path("/out/blue/bin").split();
    EXPECT_EQ(4, segments.size());
    EXPECT_EQ("/", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("blue", segments[2]);
    EXPECT_EQ("bin", segments[3]);

    segments = Path("/out/blue/bin/").split();
    EXPECT_EQ(4, segments.size());
    EXPECT_EQ("/", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("blue", segments[2]);
    EXPECT_EQ("bin", segments[3]);
}

TEST(PathTest, CurrentDirectory) {
    Path p(Path::pwd());
    EXPECT_FALSE(p.is_empty());
    EXPECT_TRUE (p.is_absolute());
    EXPECT_TRUE (p.exists());
    EXPECT_TRUE (p.is_directory());
    EXPECT_FALSE(p.is_file());
}

TEST(PathTest, CurrentExecutable) {
    Path p(Path::current_executable());
    EXPECT_FALSE(p.is_empty());
    EXPECT_TRUE(p.is_absolute());
    EXPECT_TRUE(p.exists());
    EXPECT_FALSE(p.is_directory());
    EXPECT_TRUE (p.is_file());
}

TEST(PathTest, AbsolutePath) {
    Path cwd = Path::pwd();

    Path p;
    p = Path("/out/blue/bin");
    EXPECT_TRUE(p.is_absolute());

    p = p.absolute_path();
    EXPECT_EQ("/out/blue/bin", p.get());

    p = Path("../bin").absolute_path();
    EXPECT_NE(cwd, p);
    EXPECT_TRUE(p.is_absolute());
}

TEST(PathTest, IsFile) {
    Path dir(Path::pwd());
    Path exe(Path::current_executable());
    EXPECT_TRUE (exe.is_file());
    EXPECT_FALSE(dir.is_file());
}

TEST(PathTest, IsDirectory) {
    Path dir(Path::pwd());
    Path exe(Path::current_executable());
    EXPECT_FALSE(exe.is_directory());
    EXPECT_TRUE (dir.is_directory());
}

TEST(PathTest, GetExtension) {
    Path p("/out/bin/somefile.txt");
    EXPECT_EQ(p.extension(), "txt");

    p = Path("/out/bin/somefilewithoutextension");
    EXPECT_EQ(p.extension(), "");

    p = Path("/out/bin/.tempdir/somefile.txt.bak");
    EXPECT_EQ(p.extension(), "bak");

    p = Path("/out/bin/.tempfile");
    EXPECT_EQ(p.extension(), "");

    p = Path("/out/bin/endsindot.");
    EXPECT_EQ(p.extension(), "");

    p = Path::pwd();
    EXPECT_EQ(p.extension(), "");

    p = Path();
    EXPECT_EQ(p.extension(), "");
}
#endif
