set shell=/bin/bash

"formatting
filetype plugin indent on
set number
set tabstop=4
set shiftwidth=4
set expandtab

"load template if we're opening a new .cpp file
autocmd BufNewFile  *.cpp   0r ~/.vim/templates/skeleton.cpp

"install plugins
execute pathogen#infect()

"pretty colors
syntax on
hi comment ctermfg=blue
set background=dark
colorscheme solarized

nnoremap <C-B> :Dispatch

"dispatch g++ <filename>.cpp -o <filename> with ctrl-C
command DM w<bar>Dispatch! g++-8 -std=c++17 -O2 -fconcepts %:r.cpp -o %:r
nnoremap <C-C> :DM<CR>

"open quickfix pane, then close if no errors
command FF :Copen|cw
nnoremap <C-F> :FF<CR>

"run on inp, write to out, open out
command DR Dispatch! ./%:r < inp > out
command OO bot sp out
map <C-V> :DR<CR>:OO<CR>

"automatically toggle paste mode before and after pasting
function! WrapForTmux(s)
  if !exists('$TMUX')
    return a:s
  endif
  let tmux_start = "\<Esc>Ptmux;"
  let tmux_end = "\<Esc>\\"
  return tmux_start . substitute(a:s, "\<Esc>", "\<Esc>\<Esc>", 'g') . tmux_end
endfunction

let &t_SI .= WrapForTmux("\<Esc>[?2004h")
let &t_EI .= WrapForTmux("\<Esc>[?2004l")

function! XTermPasteBegin()
  set pastetoggle=<Esc>[201~
  set paste
  return ""
endfunction

inoremap <special> <expr> <Esc>[200~ XTermPasteBegin()

"strip trailing whitespace when writing c++, java, python files
autocmd BufWritePre *.cpp,*.java,*.py %s/\s\+$//e

"simpler pane navigation
nnoremap <C-J> <C-W><C-J>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>
nnoremap <C-H> <C-W><C-H>

