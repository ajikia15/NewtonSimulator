int Calculate(class PERSONS& ps) {
    int result_size = 0;
    int max_count = 0;
    float sum_age = 0;
    int start_index = 0;
    ps.Sort();
    for (int i = 0; i <= ps.GetSize(); i++) {
        if (i == ps.GetSize() || !ps.CmpName(i, start_index)) {
            int count = i - start_index;
            float avg_age = sum_age / count;
            if (count > max_count) {
                max_count = count;
            }
            sum_age = 0;
            start_index = i;
        }
        if (i < ps.GetSize()) {
            sum_age += ps.GetAge(i);
        }
    }
    for (int i = 0; i < ps.GetSize(); i++) {
        if (i == 0 || !ps.CmpName(i, i-1)) {
            int count = 1;
            float sum_age = ps.GetAge(i);
            for (int j = i+1; j < ps.GetSize(); j++) {
                if (ps.CmpName(j, j-1)) {
                    count++;
                    sum_age += ps.GetAge(j);
                } else {
                    break;
                }
            }
            if (count == max_count) {
                ps.Copy(result_size, i);
                ps.SetAgeNA(result_size, sum_age / count);
                result_size++;
            }
        }
    }
    return result_size;
}
