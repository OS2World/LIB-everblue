/* */
do while lines('missing exports')
	a = linein('missing exports')
	parse var a '� ' nr ' � ' name
	nr2 = nr + 0
	say '	'name' = _'name' @ 'nr2
end
