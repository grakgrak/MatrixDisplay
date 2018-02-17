local function main()
	local in_filename
	local out_filename
	local fi
	local fo
	local data
	local var_name
	local builder = {}
	local pos     = 0
	local block
	local c

	if not arg or #arg < 2 then
		print("usage: "..arg[0].." in out var_name")
		return 1
	end

	in_filename = arg[1]
	fi = io.open(in_filename, "rb")
	if not fi then
		print("Failure: could not open "..in_filename.." for reading")
		return 1
	end

	out_filename = arg[2]
	fo = io.open(out_filename, "w")
	if not fo then
		print("Failure: could not open "..out_filename.." for writing")
		fi:close()
		return 1
	end

	data = fi:read("*a")
	if not data then
		print("Failure: file is empty")
		fo:close()
		fi:close()
		return 1
	end
	fi:close()

	var_name = #arg > 2 and arg[3] or string.gsub(in_filename, "%.", "_")
	builder[#builder+1] = "unsigned char PROGMEM "..var_name.."[] = {"
	builder[#builder+1] = "\n"
	pos = 1
	while pos < #data do
		block = data:sub(pos, pos+11)
		for i=1,#block do
			c = block:sub(i, i)
			builder[#builder+1] = ("%s0x%02x%s%s"):format(i==1 and "  " or "", string.byte(c), pos+i<=#data and "," or "", i<12 and pos+i<=#data and " " or "")
		end
		pos = pos+12
		builder[#builder+1] = "\n"
	end

	builder[#builder+1] = "};"
	builder[#builder+1] = "\n"
	builder[#builder+1] = "unsigned int "..var_name.."_len = "..#data..";"
	builder[#builder+1] = "\n"

	fo:write(table.concat(builder, ""))
	fo:close()

	return 0
end

os.exit(main())
