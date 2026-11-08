" Vim sytax file
" Language:		Conc (*Conc*atentative Programming language)
" Last Change:  2026-11-08

if exists("b:current_syntax")
	finish
endif

syn keyword concBuiltinSymbols proc do end
syn match   concNumber "\<[0-9]*\>"

syn match concIf /?\|,/

syn region	concString start="\"" end="\"\|$"
syn region	concComment start=";" end="$"


hi def link concBuiltinSymbols	Statement
hi def link concIf				Conditional
hi def link concNumber			Constant
hi def link concString			String
hi def link concComment			Comment

let b:current_syntax = "conc"
