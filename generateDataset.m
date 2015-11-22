function generateDataset()
% class_names = {'alive' 'all' 'boy' 'building' 'buy' 'cold' 'come' 'computer' 'cost' 'crazy' 'danger' 'deaf' 'different' 'girl' 'glove' 'go' 'God' 'joke' 'juice' 'man' 'where' 'which' 'yes' 'you' 'zero'};
class_names = {'circle' 'leftright' 'updown'};
% root_dir = './dataset';
% root_dir = './tctodd';
root_dir = './webcamdata';
direcs = dir(root_dir);
dir_list = {direcs([direcs.isdir]).name};
dir_list = dir_list(~ismember(dir_list,{'.','..'}));
dataset = [];
labels = {};
for i = 1:length(dir_list)
    dir_path = sprintf('%s/%s/*.tgd', root_dir, dir_list{i}); %tfd
    files = dir(dir_path);
    for j = 1:length(files)
        file_path = sprintf('%s/%s/%s', root_dir, dir_list{i}, files(j).name);
        cnt = 1;
        file_name = files(j).name;
        for c = 1:length(file_name)
%             if(file_name(c) >= '0' && file_name(c) <= '9')
%                 break;
%             end
            if(file_name(c) == '-')
                break;
            end
            cnt = cnt + 1;
        end
        c_name = file_name(1:cnt - 1);
        if((any(ismember(class_names, c_name))) == 0)
            continue;
        end
        M = read_mixed_csv(file_path, '\t');
        [m,n] = size(M);
        if(m < 29)
            continue;
        end
%         M = M(1:50, 1:22); % big data
        M = M(1:30, 1:10);
        M = cellfun(@str2num, M);
        dataset = cat(3, dataset, M);
        labels{end + 1} = c_name;
    end
end
tabulate(labels);
save('dataset.mat', 'dataset', 'labels');
end