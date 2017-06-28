set shell=/bin/bash

"install plugins
execute pathogen#infect()

nnoremap <C-B> :Dispatch 

"quit all buffers except first with ctrl-space
inoremap <C-Space> <C-x><C-o>
inoremap <C-@> <C-Space>
nmap <C-@> :2,99bd<CR>

"g++ <filename>.cpp -o <filename>
command DM w<bar>Dispatch! g++ -std=c++11 %:r.cpp -o %:r
nnoremap <C-C> :DM<CR>

"open quickfix, then close if no errors
command FF :Copen|cw
nnoremap <C-F> :FF<CR>

"run on inp, write to out, open out
command DR Dispatch! ./%:r < inp > out
command OO bot sp out
map <C-V> :DR<CR>:OO<CR>

"formatting
filetype plugin indent on
set number
set tabstop=4
set shiftwidth=4
set expandtab

"pretty colors
syntax on
hi comment ctermfg=blue
set background=dark
colorscheme solarized

"load template
autocmd BufNewFile  *.cpp   0r ~/.vim/templates/skeleton.cpp

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

