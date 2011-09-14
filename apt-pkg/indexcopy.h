// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
// $Id: indexcopy.h,v 1.3 2001/05/27 04:46:54 jgg Exp $
/* ######################################################################

   Index Copying - Aid for copying and verifying the index files
   
   ##################################################################### */
									/*}}}*/
#ifndef INDEXCOPY_H
#define INDEXCOPY_H

#include <vector>
#include <string>
#include <stdio.h>

using std::string;
using std::vector;

class pkgTagSection;
class FileFd;
class indexRecords;
class pkgCdromStatus;

class IndexCopy								/*{{{*/
{
   /** \brief dpointer placeholder (for later in case we need it) */
   void *d;

   protected:
   
   pkgTagSection *Section;
   
   string ChopDirs(string Path,unsigned int Depth);
   bool ReconstructPrefix(string &Prefix,string OrigPath,string CD,
			  string File);
   bool ReconstructChop(unsigned long &Chop,string Dir,string File);
   void ConvertToSourceList(string CD,string &Path);
   bool GrabFirst(string Path,string &To,unsigned int Depth);
   virtual bool GetFile(string &Filename,unsigned long long &Size) = 0;
   virtual bool RewriteEntry(FILE *Target,string File) = 0;
   virtual const char *GetFileName() = 0;
   virtual const char *Type() = 0;
   
   public:

   bool CopyPackages(string CDROM,string Name,vector<string> &List,
		     pkgCdromStatus *log);
   virtual ~IndexCopy() {};
};
									/*}}}*/
class PackageCopy : public IndexCopy					/*{{{*/
{
   protected:
   
   virtual bool GetFile(string &Filename,unsigned long long &Size);
   virtual bool RewriteEntry(FILE *Target,string File);
   virtual const char *GetFileName() {return "Packages";};
   virtual const char *Type() {return "Package";};
   
};
									/*}}}*/
class SourceCopy : public IndexCopy					/*{{{*/
{
   protected:
   
   virtual bool GetFile(string &Filename,unsigned long long &Size);
   virtual bool RewriteEntry(FILE *Target,string File);
   virtual const char *GetFileName() {return "Sources";};
   virtual const char *Type() {return "Source";};
   
};
									/*}}}*/
class TranslationsCopy							/*{{{*/
{
   protected:
   pkgTagSection *Section;

   public:
   bool CopyTranslations(string CDROM,string Name,vector<string> &List,
			 pkgCdromStatus *log);
};
									/*}}}*/
class SigVerify								/*{{{*/
{
   /** \brief dpointer placeholder (for later in case we need it) */
   void *d;

   bool Verify(string prefix,string file, indexRecords *records);
   bool CopyMetaIndex(string CDROM, string CDName, 
		      string prefix, string file);

 public:
   bool CopyAndVerify(string CDROM,string Name,vector<string> &SigList,
		      vector<string> PkgList,vector<string> SrcList);

   /** \brief generates and run the command to verify a file with gpgv */
   static bool RunGPGV(std::string const &File, std::string const &FileOut,
		       int const &statusfd, int fd[2]);
   inline static bool RunGPGV(std::string const &File, std::string const &FileOut,
			      int const &statusfd = -1) {
      int fd[2];
      return RunGPGV(File, FileOut, statusfd, fd);
   };
};
									/*}}}*/

#endif
