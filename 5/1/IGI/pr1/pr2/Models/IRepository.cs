using System;
using System.Collections.Generic;

namespace pr2.Models
{
    public interface IRepository<T>
    {
        List<T> GetAll();

        T Create(T item);
        T Read(string id);
        bool Update(string id, T item);
        bool Delete(string id);
    }
}
