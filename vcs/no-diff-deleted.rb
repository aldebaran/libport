class Svn
 def diffw! files_orig=[], options={}
   super(files_orig, options.merge(:no_diff_deleted => true))
 end
end # class Svn
