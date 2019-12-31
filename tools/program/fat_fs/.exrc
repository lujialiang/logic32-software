if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <F7> mmgUiw`ma
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cfile>"),0)
noremap <Right> 
noremap <Left> 
noremap <Down> 
noremap <Up> 
nnoremap <expr> <F10> filereadable('Makefile')?':make burn ':':!ls'
nnoremap <expr> <F9> filereadable('Makefile')?':w:make ':':!ls'
nnoremap <F8> mm"zyiw`m:!dict z
nnoremap <F7> :cnext
nnoremap <F6> :cprev
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set backspace=indent,eol,start
set fileencodings=ucs-bom,utf-8,cp936
set helplang=en
set hlsearch
set nomodeline
set path=.,/usr/include,~/Workspace/arm/stm32/newlib,/opt/STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery,/opt/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include,/opt/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include,/opt/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc,
set printoptions=paper:a4
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set shiftwidth=4
set softtabstop=4
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set tabstop=4
" vim: set ft=vim :
