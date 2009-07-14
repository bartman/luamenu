#!luamenu -lf
-- menu for uzbl window manager

local commands = {
        open = {
                command = cmd_open,
                complete = open_tab_handler,
        },
        tabopen = {
                command = cmd_tabopen,
                complete = open_tab_handler,
        },
}
local command_names = {'open', 'tabopen'}

local function is_command(name)
        return 
end

-- returns the URLs from uzbl history
local function url_history()
end

-- returns the arguments previously used with keywords
local function arg_history()
end

local function tab_handler (ev)
        if ev.cmd_index == 0 then
                luamenu.words = command_names
        elseif ev.cmd_index == 1 then
                luamenu.words = command_names + url_history()
        elseif ev.cmd_index > 2 and is_command(ev.cmd[1]) then
                luamenu.words = arg_history()
        end
end

function init ()
        luamenu.prmopt = 'uzbl:'
        luamenu.lines  = 2
        luamenu.words  = command_names
        luamenu.bind('Tab', tab_handler)
end

function arg ()
        -- handle -la command line arguments
end

function cleanup ()
        -- for writing back any history files
end

