#include "QAHtml.h"

#include "TSystem.h"

#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <vector>

namespace
{

  //___________________________________________________________________________
  std::vector<std::string> split(const char sep, const std::string& s)
  {
    std::string str = s;
    std::vector<size_t> slashes_pos;

    if (str[0] != sep)
    {
      str.insert(str.begin(), sep);
    }

    if (str[str.size() - 1] != sep)
    {
      str.push_back(sep);
    }

    for (size_t i = 0; i < str.size(); i++)
    {
      if (str[i] == sep)
      {
        slashes_pos.push_back(i);
      }
    }

    std::vector<std::string> parts;

    if (slashes_pos.size() > 0)
    {
      for (size_t i = 0; i < slashes_pos.size() - 1; i++)
      {
        parts.push_back(str.substr(slashes_pos[i] + 1,
                                   slashes_pos[i + 1] - slashes_pos[i] - 1));
      }
    }

    return parts;
  }

  //___________________________________________________________________________
  std::string join(const char sep, const std::vector<std::string>& parts)
  {
    std::string rv;
    for (size_t i = 0; i < parts.size(); ++i)
    {
      rv += parts[i];
      if (i + 1 < parts.size())
      {
        rv += sep;
      }
    }
    return rv;
  }
}  // namespace

//_____________________________________________________________________________
QAHtml::QAHtml(const char* topdir)
{
  if (topdir)
  {
    fHtmlDir = topdir;
  }
  else
  {
    fHtmlDir = "./";
  }
}

//_____________________________________________________________________________
void QAHtml::addMenu(const std::string& header, const std::string& path,
                     const std::string& relfilename)
{
  std::ostringstream menufile;

  menufile << fHtmlRunDir << "/menu";

  std::ifstream in(menufile.str().c_str());

  if (!in.good())
  {
    if (Verbosity())
    {
      std::cout << "File " << menufile.str() << " does not exist."
                << "I'm creating it now" << std::endl;
    }
    std::ofstream out(menufile.str().c_str());
    out.close();
  }
  else
  {
    if (Verbosity())
    {
      std::cout << "Reading file " << menufile.str() << std::endl;
    }
  }

  // we read back the old menu file...
  std::vector<std::string> lines;
  char str[1024];
  while (in.getline(str, 1024, '\n'))
  {
    lines.emplace_back(str);
  }
  in.close();

  // ... we then append the requested new entry...
  std::ostringstream sline;
  sline << header << "/" << path << "/" << relfilename;

  lines.push_back(sline.str());

  // ... and we sort this out...
  sort(lines.begin(), lines.end());

  // ... and we remove duplicates lines...
  std::set<std::string> olines;
  copy(lines.begin(), lines.end(),
       std::insert_iterator<std::set<std::string> >(olines, olines.begin()));

  // ... and finally we write the full new menu file out.
  std::ofstream out(menufile.str().c_str());
  copy(olines.begin(), olines.end(), std::ostream_iterator<std::string>(out, "\n"));
  out.close();

  // --end of normal menu generation--

  // -- For those who do not have javascript (and thus the menu file
  // created by addMenu will be useless) or
  // in case cgi script(s) won't be allowed for some reason,
  // make a plain html menu file too.
  plainHtmlMenu(olines);
}

//_____________________________________________________________________________
void QAHtml::plainHtmlMenu(const std::set<std::string>& olines)
{
  std::ostringstream htmlmenufile;

  htmlmenufile << fHtmlRunDir << "/menu.html";

  // First get the list of directories found in menu file above (contained
  // in olines set). The olines are of the form:
  // D1/D2/TITLE/link (where link is generally somefile.gif)
  // The dir in this case is D1/D2, which is why we look for 2 slashes
  // below (the one before TITLE and the one before link).
  std::set<std::string> dirlist;
  std::set<std::string>::const_iterator it;
  for (it = olines.begin(); it != olines.end(); ++it)
  {
    const std::string& line = *it;
    std::string::size_type pos = line.find_last_of('/');
    pos = line.substr(0, pos).find_last_of('/');
    std::string dir = line.substr(0, pos);
    std::vector<std::string> parts = split('/', dir);
    for (size_t i = 0; i <= parts.size(); ++i)
    {
      dir = join('/', parts);
      dirlist.insert(dir);
      parts.pop_back();
    }
  }

  // We now generate the menu.html file.
  std::ofstream out(htmlmenufile.str().c_str());
  if (!out.good())
  {
    std::cout << " cannot open output file "
              << htmlmenufile.str() << std::endl;
    return;
  }

  for (it = dirlist.begin(); it != dirlist.end(); ++it)
  {
    // in the example above, dir is D1/D2
    const std::string& dir = *it;
    int nslashes = count(dir.begin(), dir.end(), '/') + 1;
    std::string name = dir;
    std::string::size_type pos = dir.find_last_of('/');
    if (pos < dir.size())
    {
      name = dir.substr(pos + 1);
    }
    else
    {
      out << "<HR><BR>\n";
    }
    out << "<H" << nslashes << ">" << name
        << "</H" << nslashes << "><BR>\n";

    // We then loop on all the olines, and for those matching the
    // dir pattern, we generate link <A HREF="link">TITLE</A>
    std::set<std::string>::const_iterator it2;
    for (it2 = olines.begin(); it2 != olines.end(); ++it2)
    {
      const std::string& line = *it2;
      pos = line.find_last_of('/');
      pos = line.substr(0, pos).find_last_of('/');
      std::string ldir = line.substr(0, pos);
      if (ldir == dir)  // we get a matching line
      {
        std::string sline = line.substr(dir.size() + 1);
        // in the example above, sline is TITLE/link...
        pos = sline.find('/');
        // ...which we split at the slash pos
        if (pos < sline.size())
        {
          out << "<A HREF=\""
              << sline.substr(pos + 1) << "\">"
              << sline.substr(0, pos) << "</A><BR>\n";
        }
      }
    }
  }
  out.close();
}

//_____________________________________________________________________________
void QAHtml::namer(const std::string& header,
                   const std::string& basefilename,
                   const std::string& ext,
                   std::string& fullfilename,
                   std::string& filename)
{
  std::ostringstream sfilename;

  sfilename << header << "_";
  if (!basefilename.empty())
  {
    sfilename << basefilename << "_";
  }
  sfilename << runNumber() << "." << ext;

  std::ostringstream sfullfilename;

  sfullfilename << fHtmlRunDir << "/" << sfilename.str();

  fullfilename = sfullfilename.str();
  filename = sfilename.str();

  if (Verbosity())
  {
    std::cout << "namer: header=" << header
              << " basefilename=" << basefilename << " ext=" << ext
              << std::endl
              << "fullfilename=" << fullfilename
              << " filename=" << filename
              << std::endl;
  }
}

//_____________________________________________________________________________
std::string
QAHtml::registerPage(const std::string& header,
                     const std::string& path,
                     const std::string& basefilename,
                     const std::string& ext)
{
  std::string fullfilename;
  std::string filename;
  static std::string saveheader = "";
  if (saveheader != header)
  {
    runInit();
    saveheader = header;
  }
  namer(header, basefilename, ext, fullfilename, filename);
  addMenu(header, path, filename);
  return fullfilename;
}

//_____________________________________________________________________________
void QAHtml::runInit()
{
  // Check if html output directory for this run exist.
  // If not create it.
  // Then check (and create if necessary) the "menu" template file.

  std::ostringstream fulldir;
  fulldir << fHtmlDir << "/" << runtype << "/"
          << runRange() << "/" << runNumber();

  fHtmlRunDir = fulldir.str();
  DIR* htdir = opendir(fulldir.str().c_str());
  if (!htdir)
  {
    std::vector<std::string> mkdirlist;
    mkdirlist.push_back(fulldir.str());
    std::string updir = fulldir.str();
    std::string::size_type pos1;
    while ((pos1 = updir.rfind('/')) != std::string::npos)
    {
      updir.erase(pos1, updir.size());
      htdir = opendir(updir.c_str());
      if (!htdir)
      {
        mkdirlist.push_back(updir);
      }
      else
      {
        closedir(htdir);
        break;
      }
    }
    while (mkdirlist.rbegin() != mkdirlist.rend())
    {
      std::string md = *(mkdirlist.rbegin());
      if (Verbosity())
      {
        std::cout << "Trying to create dir " << md << std::endl;
      }
      std::filesystem::perms permissions = std::filesystem::perms::owner_all | std::filesystem::perms::group_all | std::filesystem::perms::group_exec | std::filesystem::perms::others_read | std::filesystem::perms::others_exec;
      if (std::filesystem::create_directory(md))
      {
	if (Verbosity())
	{
	  std::cout << "created " << md << std::endl;
	}
        char* onlprod_real_html = getenv("QA_REAL_HTML");
        if (!onlprod_real_html)
        {
          std::filesystem::permissions(md, permissions);
          set_group_sticky_bit(md);
        }
      }
      else
      {
        std::cout << "Error creating directory " << md << std::endl;
        fHtmlRunDir = fHtmlDir;
        break;
      }
      mkdirlist.pop_back();
    }
  }
  else
  {
    closedir(htdir);
  }
  if (Verbosity())
  {
    std::cout << "OK. fHtmlRunDir=" << fHtmlRunDir << std::endl;
  }
}

//_____________________________________________________________________________
std::string
QAHtml::runRange()
{
  const int range = 1000;
  int start = runNumber() / range;

  std::ostringstream s;

  s << "run_" << std::setw(10) << std::setfill('0') << start * range
    << "_" << std::setw(10) << std::setfill('0') << (start + 1) * range;

  return s.str();
}

void QAHtml::RunType(const std::string& rtyp)
{
  runtype = rtyp;
  transform(runtype.begin(), runtype.end(), runtype.begin(), (int (*)(int)) tolower);
  return;
}

void QAHtml::set_group_sticky_bit(const std::filesystem::path& dir)
{
  struct stat st
  {
  };
  if (stat(dir.c_str(), &st) != 0)
  {
    std::cout << "Failed to stat directory: " << std::strerror(errno) << std::endl;
    return;
  }

  // Add the setgid bit to the existing permissions
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  mode_t new_mode = st.st_mode | S_ISGID;

  if (chmod(dir.c_str(), new_mode) != 0)
  {
    std::cerr << "Failed to set group sticky bit: " << std::strerror(errno) << std::endl;
  }
  else
  {
    if (Verbosity() > 0)
    {
      std::cout << "Group sticky bit set for directory: " << dir << std::endl;
    }
  }
}
