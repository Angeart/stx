require 'find'
require 'fileutils'
require 'optparse'

print '[current]'
puts Dir.pwd

$options = {}
OptionParser.new do |opt|
    opt.on('-n', '--not-remove', 'not remove before copy headers'){|v| $options[:n] = v}
    opt.on('-c [VALUE]', '--current-directory', 'change current directory, ignored this option if enabled -j'){|v| $options[:c] = v}
    opt.on('-j', '--just-in-current', 'run in directory of exists executable this file.'){|v| $options[:j] = v}
    opt.on('-o VALUE', '--out-directory', 'output headers directory.'){|v| $options[:o] = v}
    opt.parse!(ARGV)
end

if $options[:c] != nil && !$options[:j]
    print '[change current]'
    Dir.chdir(File::extend_path($options[:c]))
    puts Dir.pwd
end

if $options[:j]
    print '[change current]'
    Dir.chdir(File.dirname(__FILE__))
    puts Dir.pwd
end

def clear(path)
    print '[remove]'
    if !$options[:n] && File.exists?(path)
        puts path
        FileUtils.rm_rf(path)
    else
        puts 'skipped..'
    end
    print '[create]'
    unless File.exists?(path)
        puts path
        FileUtils.mkdir_p(path)
    else
        puts 'skipped..'
    end
end

def headers()
    include_directory = $options[:o]
    find_directory = './'
    match_pattern = /.hp{0,2}$/
    replace_pattern = /^\.{1}/
    clear(include_directory)
    puts '[copy]'
    Find.find(find_directory) {|path|
        if path.match(match_pattern)
            dst = path.sub(replace_pattern,include_directory)
            dst_directory = File.dirname(dst)
            FileUtils.mkdir_p(dst_directory) unless File.exists?(dst_directory)
            FileUtils.cp(path,dst,{:preserve => true,:verbose => true})
        end
    }
end

headers()
