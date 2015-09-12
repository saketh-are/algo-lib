set shell=/bin/bash

"install plugins
execute pathogen#infect()

nnoremap <C-X> :wq<CR>
nnoremap <C-B> :Dispatch 
inoremap jk <Esc>

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
command DR Dispatch ./%:r < inp > out
command OO bot sp out
map <C-V> :DR<CR>:OO<CR>

"formatting
filetype plugin indent on
set number
set tabstop=4
set shiftwidth=4
set expandtab
set pastetoggle=<F2>

"pretty colors
syntax on
hi comment ctermfg=blue
let g:solarized_termcolors=256
let g:solarized_termtrans=1
set background=light
colorscheme solarized

"load template
autocmd BufNewFile  *.cpp   0r ~/.vim/templates/skeleton.cpp

"break arrow key addiction
noremap <Up> <NOP>
noremap <Down> <NOP>
noremap <Left> <NOP>
noremap <Right> <NOP>
inoremap <Up> <NOP>
inoremap <Down> <NOP>
inoremap <Left> <NOP>
inoremap <Right> <NOP>

