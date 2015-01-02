let g:nerdtree_tabs_open_on_console_startup=1
set listchars=eol:$,tab:>-,trail:~,extends:>,precedes:<

" vimcppcheck.vim
"  ===================================================================
"  Code Checking with cppcheck (1)
"  Thanks to Mr. Ingo Karkat
"  http://stackoverflow.com/questions/19157270/vim-cppcheck-which-errorformat-to-use
"  ===================================================================

function! Cppcheck_1()
  setlocal makeprg=cppcheck\ --enable=all\ %
  " earlier it was: " setlocal errorformat=[%f:%l]:%m
  " fixed by an advise by Mr. Ingo Karkat
  setlocal errorformat+=[%f:%l]\ ->\ %m,[%f:%l]:%m
  let curr_dir = expand('%:h')
  if curr_dir == ''
    let curr_dir = '.'
  endif
  echo curr_dir
  execute 'lcd ' . curr_dir
  execute 'make'
  execute 'lcd -'
  exe   ":botright cwindow"
  :copen
endfunction
